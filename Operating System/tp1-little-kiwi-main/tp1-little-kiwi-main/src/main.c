#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <memory.h>
#include <sys/wait.h>

#define true 1
#define false 0
#define bool int

typedef int error_code;

#define ERROR (-1)
#define HAS_ERROR(code) ((code) < 0)
#define NULL_TERMINATOR '\0'

enum op {   //todo these are your custom shell operators. You might want to use them to represent &&, ||, & and "no operator"
    BIDON, NONE, OR, AND, ALSO    //BIDON is just to make NONE=1, BIDON is unused
};

typedef struct command {    //todo you might want to use this to represent the different commands you find on a line
    char **call;
    enum op operator;
    struct command *next;
    int count;
    bool also;
} command;
//hint hint: this looks suspiciously similar to a linked list we saw in the demo. I wonder if I could use the same ideas here??

void freeStringArray(char **arr) {  //todo probably add this to free the "call" parameter inside of command
    if (arr != NULL) {
        for (int i = 0; arr[i] != NULL; i++) {
            free(arr[i]);
        }
        free(arr);
    }
}

error_code free_command_list(command *list){
    if(list == NULL){
        return ERROR;
    }

    command *tmp;
    while(list->next != NULL){
        tmp = list;
        list = list->next;
        freeStringArray(tmp->call);
        free(tmp);
    }
    freeStringArray(list->call);
    free(list);

    return 0;
}

error_code readline(char **out) {   //todo this is pretty barebones, you must complete it
    if(out == NULL){
        return ERROR;
    }

    size_t size = 10;                       // size of the char array
    char *line = malloc(sizeof(char) * size);       // initialize a 255-char line
    if (line == NULL) return ERROR;   // if we can't, terminate because of a memory issue

    for (int at = 0; 1; at++) {
        if (at >= size) {
            size = 2*size;
            char *tempPtr = realloc(line, size * sizeof(char));

            if (tempPtr == NULL) {
                free(line);
                return ERROR;
            }

            line = tempPtr;
        }

        int nextChar = getchar();

        if (nextChar == EOF) { // if the next char is EOF, terminate
            free(line);
            return ERROR;
        }

        char ch = (char) nextChar;

        if (ch == '\n') {        // if we get a newline
            line[at] = NULL_TERMINATOR;    // finish the line with return 0
            break;
        }

        line[at] = ch; // sets ch at the current index and increments the index
    }

    out[0] = line;

    return 0;
}

char **parse_call(char *call){
    size_t size = 11;
    char **ret = malloc(size*sizeof(char*));
    if(ret == NULL){
        return NULL;
    }

    size_t l = strlen(call);

    size_t i = 0,j = 0,k = 0;
    ret[0] = malloc((l + 1)*sizeof(char));

    if(ret[0] == NULL){
        free(ret);
        return NULL;
    }

    char c;
    for(i = 0; i < l; i++){
        c = call[i];
        if(c == ' '){

            ret[j][k] = NULL_TERMINATOR;
            j++;
            while(call[i + 1] == ' ') i++;

            if(j >= size){
                size *= 2;
                char **tempPtr = realloc(ret, size * sizeof(char *));

                if (tempPtr == NULL) {
                    for(k = 0; k < j; k++){
                        free(ret[k]);
                    }
                    free(ret);
                    return NULL;
                }

                ret = tempPtr;
            }

            ret[j] = malloc((l - i + 1)*sizeof(char));

            if(ret[j] == NULL){
                for(int n = 0; n < j; n++){
                    free(ret[n]);
                }
                free(ret);
                return NULL;
            }

            k = 0;
        }else{
            ret[j][k] = c;

            k++;
        }
    }

    ret[j][k] = NULL_TERMINATOR;

    j++;
    if(j >= size){
        char **tempPtr = realloc(ret, (size+1) * sizeof(char *));

        if (tempPtr == NULL) {
            for(k = 0; k < j; k++){
                free(ret[k]);
            }
            free(ret);
            return NULL;
        }

        ret = tempPtr;
    }

    ret[j] = NULL;

    return ret;
}

error_code parse_line(char *line, command **commands){
    if(line == NULL || commands == NULL){
        return ERROR;
    }

    command *cmd = NULL;
    command *last;
    command *new = NULL;
    last = cmd;

    size_t l = strlen(line);
    size_t i;
    char *snippet;

    int r, n;
    char c;
    for(i = 0; i < l ; i++){
        c = line[i];
        if(c == ' '){
            continue;
        }else if(c == '|'){
            if(i + 1 < l && line[i+1] == '|' && last->operator == NONE){
                i++;
                last->operator = OR;
            }else{
                free_command_list(cmd);
                return ERROR;
            }
        }else if(c == '&'){
            if(i+1 >= l){
                if(cmd != NULL && last->operator == NONE){
                    cmd->also = true;
                }else{
                    if(cmd != NULL){
                        free_command_list(cmd);
                    }
                    return ERROR;
                }
            }else if(i + 1 < l && line[i+1] == '&' && last->operator == NONE){
                i++;
                last->operator = AND;
            }else{
                free_command_list(cmd);
                return ERROR;
            }
        }else{
            new = malloc(sizeof(command));
            snippet = malloc((l - i + 1)*sizeof(char));
            if(new == NULL){
                free_command_list(cmd);
                free(snippet);
                return ERROR;
            }

            if(cmd == NULL){
                cmd = new;
                last = cmd;
                *commands = cmd;
            }else{
                last->next = new;
                last = new;
            }
            new->next = NULL;
            new->also = false;
            new->call = NULL;
            new->count = 0;
            new->operator = NONE;
            if(c == 'r' && sscanf(line + i, "r%d(%[^)])", &r, snippet) == 2 && r > 0){
                while(line[++i] != ')'){
                    if(line[i] == '&' || line[i] == '|'){
                        free_command_list(cmd);
                        free(snippet);
                        return ERROR;
                    }
                }

                new->count = r;
                new->call = parse_call(snippet);
            }else{
                new->count = 1;

                n = i;
                while(i < l && line[i] != '|' && line[i] != '&'){
                    snippet[i-n] = line[i];
                    if(line[i] == ' '){
                        while(line[i+1] == ' '){
                            i++;
                            n++;
                        }
                    }
                    i++;
                }

                if(snippet[i-n-1] == ' '){
                    snippet[i-n-1] = NULL_TERMINATOR;
                }else{
                    snippet[i-n] = NULL_TERMINATOR;
                }
                i--;

                new->call = parse_call(snippet);
            }
            free(snippet);
            new = NULL;
        }
    }

    return 0;
}

/******************************************
 * function : call_command
 * @param current_cmd
 *
 *comment : this function is used to call a command
 *
 */

bool call_command(command *current_cmd) {
    bool ret = true;
    if (current_cmd->count > 0) {      // if we have at least one command
        pid_t pid = 0;
        int childExitCode = -1;     // the exit code of the child

        for (int i = 0; i < current_cmd->count; i++) {
            pid = fork();

            if (pid < 0) {        // forking failed
                return pid;
            }

            if (pid == 0) {  // Child fork (succeeded)
                char *nomCommand = current_cmd->call[0];

                if(strcmp(nomCommand, "exit") == 0) {
//                    close_alsos(alsos);
                    exit(0);
                }

                // Execute command and kill child
                int status_code = execvp(nomCommand, current_cmd->call);
                if (status_code == -1) {
                    // the command does not exist
                    printf("%s: command not found\n", nomCommand);
                    exit(status_code);
                }

            } else {
                waitpid(pid, &childExitCode, 0); // Wait for the child process to exit.
                // TODO: could return WIFEXITED instead.
                if (!WIFEXITED(childExitCode) || WEXITSTATUS(childExitCode) != 0) { // if the child did not terminate normally
                    ret = false;
                }
            }
        }
    }

    return ret;
}

void close_alsos(pid_t *alsos){
    int tmp;
    for(size_t i = 0; alsos[i] > 0; i++){
        waitpid(alsos[i], &tmp,0);
    }
    free(alsos);
}

void kill_alsos(pid_t *alsos){
    for(size_t i = 0; alsos[i] > 0; i++){
        kill(alsos[i], 0);
    }
    free(alsos);
}

/******************************************
 * function : call_line
 * @param cmd
 *
 *comment : this function  call the full  commands , cmd is the head of the linked command list
 *
 */

error_code call_line(command *cmd, pid_t **alsos, size_t *pos) {
    if(cmd == NULL){
        return ERROR;
    }

    bool in_also = cmd->also;
    command *start = cmd;
    pid_t *alsos_v = *alsos;
    size_t pos_v = *pos;

    // Deal with &.
    if(in_also){
        pid_t pid;
        pid = fork();
        int childStatus;

        if(pid > 0){ // Parent thread.

            if(alsos_v[pos_v] == -1){
                pid_t *tmpPtr = realloc(alsos_v,2*(pos_v+1));

                if(tmpPtr == NULL){
                    waitpid(pid,&childStatus,0);

                    free_command_list(start);
                    close_alsos(alsos_v);
                    exit(1);
                }

                alsos_v = tmpPtr;

                for(size_t i = pos_v; i < 2*(pos_v+1); i++){
                    alsos_v[i] = 0;
                }
                alsos_v[2*pos_v + 1] = -1;

                *alsos = tmpPtr;
            }

            alsos_v[pos_v] = pid;
            *pos += 1;

            return 0;
        }else if(pid < 0){ // Thread failed.
            free_command_list(start);
            close_alsos(alsos_v);
            exit(1);
        }
    }

    bool status = true;
	bool exit_status = true;
    bool or_status = false;
    bool and_status = false;
    enum op prev_op = NONE;    do{
        if ((prev_op != OR && prev_op != AND) || (prev_op == OR && !or_status) || (prev_op == AND && and_status)) {
            exit_status = call_command(cmd);
        }

        // Deal with && and || operator.
        switch (cmd->operator) {
            case AND:
                status = exit_status;
                and_status = exit_status;
                or_status = false;
                break;
            case OR:
                status = !exit_status;
                or_status = exit_status;
                and_status = false;
                break;
            default:
                break;
        }




		prev_op = cmd->operator;
        cmd = cmd->next;
    }while(cmd != NULL);

    if(in_also){
        free_command_list(start);
        free(alsos_v);
        exit(1 - status);
    }else{
        return status - 1;
    }
}

int main(void) {
    error_code err;
    char *line;

    while (true) {
        // Initialise l'array de PIDs.
        pid_t *alsos = malloc(2*sizeof(pid_t));
        if (!alsos) {
            exit(ERROR);
        }
        alsos[0] = 0;
        alsos[1] = -1; // End of PIDs' array.
        size_t pos = 0; // Keep track des PIDs.

        err = readline(&line);    //todo what about error_code?
        if (err == ERROR) {
            printf("Error reading line.\n");
            close_alsos(alsos);
            exit(1);
        }

        if(strlen(line) == 0){
            free(line);
            close_alsos(alsos);
            continue;
        }

        // Exit Command.
        if(strcmp(line, "exit") == 0) {
            free(line);
            kill_alsos(alsos); // Children must die!!
            exit(0);
        }

        command *commands = NULL;
        err = parse_line(line,&commands);

        if(err == ERROR || commands == NULL){
            printf("Ill-formed line %s.\n",line);
            close_alsos(alsos);
            free(line);
            continue;
        }
        free(line);

        call_line(commands,&alsos,&pos);

        free_command_list(commands);
        free(alsos);
    }

    //todo probably add other functions for different parts of the homework...
}