#include <stdlib.h>
#include <stdio.h>
#include "main.h"


typedef unsigned char byte;
typedef int error_code;

#define ERROR (-1)
#define HAS_ERROR(code) ((code) < 0)
#define HAS_NO_ERROR(code) ((code) >= 0)

/**
 * Cette fonction compare deux chaînes de caractères.       
 * @param p1 la première chaîne
 * @param p2 la deuxième chaîne
 * @return le résultat de la comparaison entre p1 et p2. Un nombre plus petit que
 * 0 dénote que la première chaîne est lexicographiquement inférieure à la deuxième.
 * Une valeur nulle indique que les deux chaînes sont égales tandis qu'une valeur positive
 * indique que la première chaîne est lexicographiquement supérieure à la deuxième.
 */
int strcmp(char *p1, char *p2) {
    char *s1 = (char *) p1;
    char *s2 = (char *) p2;
    char c1, c2;
    do {
        c1 = (char) *s1++;
        c2 = (char) *s2++;
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);
    return c1 - c2;
}

// Code to correctly free a transition
void free_transition(transition **t){
    if((*t)->current_state != NULL){
        free((*t)->current_state);
        (*t)->current_state = NULL;
    }

    if((*t)->next_state != NULL){
        free((*t)->next_state);
        (*t)->next_state = NULL;
    }
    
    free(*t);
    *t = NULL;
}

/**
 * Ex. 1: Calcul la longueur de la chaîne passée en paramètre selon
 * la spécification de la fonction strlen standard
 * @param s un pointeur vers le premier caractère de la chaîne
 * @return le nombre de caractères dans le code d'erreur, ou une erreur
 * si l'entrée est incorrecte
 */
error_code strlen2(char *s) {
    if(s == NULL){
        return ERROR;
    }else{
        int len = 0;
        while(((char) *s++) != '\0'){
            len++;
        }

        return len;
    }
}

/**
 * Ex.2 :Retourne le nombre de lignes d'un fichier sans changer la position
 * courante dans le fichier.
 * @param fp un pointeur vers le descripteur de fichier
 * @return le nombre de lignes, ou -1 si une erreur s'est produite
 */
error_code no_of_lines(FILE *fp) {
    if(fp == NULL){
        return ERROR;
    }

    long pos = ftell(fp);

    //printf("Position initiale : %ld\n",pos);

    char c = (char) fgetc(fp);
    char last;
    int n_lines = 0;

    if(c == EOF){
        return 0;
    }else if(c == '\n'){
        n_lines++;
    }

    while((c = (char) fgetc(fp)) != EOF){
        if(c == '\n'){
            n_lines++;
        }
        last = c;
    }

    if(last == '\n'){
        n_lines--;
    }

    fseek(fp,pos,SEEK_SET);

    return ++n_lines;
}

/**
 * Ex.3: Lit une ligne au complet d'un fichier
 * @param fp le pointeur vers la ligne de fichier
 * @param out le pointeur vers la sortie
 * @param max_len la longueur maximale de la ligne à lire
 * @return le nombre de caractère ou ERROR si une erreur est survenue
 */
error_code readline(FILE *fp, char **out, size_t max_len) {
    if(out == NULL || fp == NULL){
        return ERROR;
    }
    
    char* line = malloc((max_len+1)*sizeof(char)); // null-terminated

    if(line == NULL){
        return ERROR;
    }

    char c;
    int curr_len = 0;

    char complete = 0;

    while(curr_len < max_len){
        c = (char) fgetc(fp);
        if(c == '\n' || c == EOF){
            line[curr_len] = '\0';
            complete = 1;
            break;
        }else{
            line[curr_len++] = c;
        }
    }

    if(complete == 0){
        return ERROR;
    }

    *out = line;

    return 0;
}

/**
 * Ex.4: Copie un bloc mémoire vers un autre
 * @param dest la destination de la copie
 * @param src  la source de la copie
 * @param len la longueur (en byte) de la source
 * @return nombre de bytes copiés ou une erreur s'il y a lieu
 */
error_code memcpy2(void *dest, void *src, size_t len) {
    if(src == NULL || dest == NULL){
        return ERROR;
    }

    char *d = (char *) dest;
    const char *s = (char *) src;

    for(size_t i = 0; i < len; i++){
        d[i] = s[i];
    }
    
    return len;
}

/**
 * Ex.5: Analyse une ligne de transition
 * @param line la ligne à lire
 * @param len la longueur de la ligne
 * @return la transition ou NULL en cas d'erreur
 */
transition *parse_line(char *line, size_t len) {
    transition *ret = malloc(sizeof(transition));

    ret->current_state = malloc(6*sizeof(char));
    ret->next_state = malloc(6*sizeof(char));

    //printf("%s\n",line);

    char *current_state = malloc(len*sizeof(char));
    char *next_state = malloc(len*sizeof(char));

    int success = sscanf(line,"(%[^,],%c)->(%[^,],%c,%c)",current_state,&(ret->read),next_state,&(ret->write),&(ret->movement));

    //printf("%d\n",success);

    if(success == EOF || success != 5 || strlen2(current_state) > 5 || strlen2(next_state) > 5){
        free(current_state);
        free(next_state);
        free_transition(&ret);
        return NULL;
    }

    memcpy2(ret->current_state,current_state,(strlen2(current_state)+1)*sizeof(char));
    memcpy2(ret->next_state,next_state,(strlen2(next_state)+1)*sizeof(char));

    free(current_state);
    free(next_state);

    switch(ret->movement){
        case 'G':{
            ret->movement = -1;
            break;
        }
        case 'R':{
            ret->movement = 0;
            break;
        }
        case 'D':{
            ret->movement = 1;
            break;
        }
        default:{
            free_transition(&ret);
            return NULL;
        }
    }

    return ret;
}


char *expand_tape(char* tape, int *i, int *tape_start, int *tape_end, int *tape_len){
    char *new_tape = malloc(2*(*tape_len)*sizeof(char));

    if(new_tape == NULL){
        return new_tape;
    }

    int offset = (*tape_len)/2;

    for(int j = 0; j < *tape_len; j++){
        new_tape[j+offset] = tape[j];
    }

    *i += offset;
    *tape_start += offset;
    *tape_end += offset;
    *tape_len *= 2;

    free(tape);

    return new_tape;
}


/**
 * Ex.6: Execute la machine de turing dont la description est fournie
 * @param machine_file le fichier de la description
 * @param input la chaîne d'entrée de la machine de turing
 * @return le code d'erreur
 */
error_code execute(char *machine_file, char *input) {
    printf("Validating file...\n");
    FILE *fp = fopen(machine_file,"r");

    if(fp == NULL){ // File does not exist
        printf("File does not exist.\n");
        return ERROR;
    }

    error_code n_lines = no_of_lines(fp);
    if(n_lines < 4){ // At least 4 lines
        printf("File has less than 4 lines.\n");
        return ERROR;
    }

    printf("Validating input...\n");
    error_code input_len = strlen2(input);
    if(input_len < 0){ // No input
        printf("Input is empty.\n");
        return ERROR;
    }

    // General setup for file processing
    int n_transitions = n_lines - 3;
    char *q0, *current_state, *accept_state, *refuse_state;
    char **line = malloc(sizeof(char *));
    transition **transitions = malloc(n_transitions*sizeof(transition *));

    printf("Reading state information...\n");
    // Read current state
    error_code error = readline(fp,line,5); // Max : (aaaaa,b)->(ccccc,d,e) (len 5+5+4+3+3+2 = 22)
    if(error < 0 || strlen2(*line) == 0){
        printf("Problem reading initial state line.\n");
        return ERROR;
    }
    current_state = *line;
    q0 = current_state;

    // Read accept state
    error = readline(fp,line,5); // Max : 5 caractères par état
    if(error < 0 || strlen2(*line) == 0){
        printf("Problem reading accepting state line.\n");
        return ERROR;
    }
    accept_state = *line;

    // Read refuse state
    error = readline(fp,line,5); // Max : 5 caractères par état
    if(error < 0 || strlen2(*line) == 0){
        printf("Problem reading refusing state line.\n");
        return ERROR;
    }
    refuse_state = *line;

    printf("Reading transitions...\n");
    // Read transitions
    int i;
    for(i = 0; i < n_transitions; i++){
        error = readline(fp,line,22); // Max : (aaaaa,b)->(ccccc,d,e) (len 5+5+4+3+3+2 = 22)
        if(error < 0){
            printf("Problem reading transition line.\n");
            return ERROR;
        }
        transitions[i] = parse_line(*line,strlen2(*line));
        if(transitions[i] == NULL){
            printf("Transition line %s could not be parsed.\n",*line);
            return ERROR;
        }
        free(*line); // We don't need theses strings anymore
    }

    free(line);

    printf("Setting up tape for length %d...\n",input_len);
    // Tape setup
    int tape_len = 2*input_len;
    char *tape;
    if(input_len == 0){
        tape = malloc(sizeof(char));
    }else{
        tape = malloc(2*input_len*sizeof(char));
    }

    int tape_start = input_len/2;
    int tape_end = input_len + input_len/2;
    char c;

    for(i = tape_start; i < tape_end; i++){
        tape[i] = input[i-tape_start];
    }

    i = tape_start;

    if(input_len == 0){
        // Empty start equivalent to ' ';
        i = 0;
        tape[0] = ' ';
        input_len = 1;
        tape_end = 1;
        tape_len = 1;
    }

    int j;
    transition *current_transition, *candidate_transition;

    printf("Running...\n");
    // Machine loop
    while(strcmp(current_state,accept_state) != 0 && strcmp(current_state,refuse_state) != 0){
        // Look for known transition
        c = tape[i];
        current_transition = NULL;

        for(j = 0; j < n_transitions; j++){
            candidate_transition = transitions[j];

            if(strcmp(candidate_transition->current_state,current_state) == 0 && candidate_transition->read == c){
                current_transition = candidate_transition;
            }
        }

        if(current_transition == NULL){ // No known transition found
            printf("Unknown transition for state %s and character \"%c\" (ASCII code %d) (i = %d, tape_start = %d, tape_end = %d)\n",current_state, c, (int) c, i, tape_start, tape_end);
            return ERROR;
        }

        /*printf("%c : %s,%c -> %s,%c,%d\n",c,
                                        current_transition->current_state,
                                        current_transition->read,
                                        current_transition->next_state,
                                        current_transition->write,
                                        (int)current_transition->movement);*/

        tape[i] = current_transition->write;
        current_state = current_transition->next_state;

        switch(current_transition->movement){
            case 0:{
                if(current_transition->read == current_transition->write){
                    // Super basic obvious loop detection
                    printf("Loop detected. Exiting.");
                    return ERROR;
                }
                break;
            }
            case 1:{
                if(i + 1 >= tape_end){
                    if(tape_end == tape_len){
                        tape = expand_tape(tape,&i,&tape_start,&tape_end,&tape_len);
                        
                        if(tape == NULL){
                            return ERROR;
                        }
                    }

                    tape[tape_end++] = ' ';
                }
                break;
            }
            case -1:{
                if(i == tape_start){
                    if(tape_start == 0){
                        tape = expand_tape(tape,&i,&tape_start,&tape_end,&tape_len);

                        if(tape == NULL){
                            return ERROR;
                        }
                    }

                    tape[tape_start--] = ' ';
                }
                break;
            }
            default:{
                printf("Unknown movement %d. Exiting.",(int) current_transition->movement);
                return ERROR;
            }
        }

        i += current_transition->movement;

        //printf("%d,%d,%d\n",i,tape_start,tape_end);
    }

    tape[tape_end] = '\0';

    error_code ret = ERROR;

    if(strcmp(current_state,accept_state) == 0){
        printf("Accepted with final tape content %s\n", tape + tape_start);
        ret = 1;
    }else{
        printf("Refused with final tape content %s\n", tape + tape_start);
        ret = 0;
    }

    free(tape);
    for(i = 0; i < n_transitions; i++){
        free_transition(&transitions[i]);
    }
    free(transitions);
    free(q0);
    free(accept_state);
    free(refuse_state);
    fclose(fp);

    return ret;
}

// ATTENTION! TOUT CE QUI EST ENTRE LES BALISES ༽つ۞﹏۞༼つ SERA ENLEVÉ! N'AJOUTEZ PAS D'AUTRES ༽つ۞﹏۞༼つ

// ༽つ۞﹏۞༼つ

int main() {
// Vous pouvez ajouter des tests pour les fonctions ici
    
    error_code ret = execute("youre_gonna_go_far_kid","");

    printf("%d\n",ret);

    return 0;
}

// ༽つ۞﹏۞༼つ