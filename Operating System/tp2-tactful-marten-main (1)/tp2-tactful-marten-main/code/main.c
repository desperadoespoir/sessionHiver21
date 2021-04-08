#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include "blocking_q.h"
#include "main.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#define TODO printf("TODO");

#define TASK_A_T (5 * 1000)
#define TASK_B_T (10 * 1000)
#define TASK_C_T (15 * 1000)
#define TASK_D_T (20 * 1000)

#define PROCESSOR_COUNT 4

#define POISON_PILL 'K'

/**
 * Code executed by task A
 */
long task_a() {
    printf("Task A starting...\n");
    sleep(5);
    printf("Task A ending...\n");
    return TASK_A_T;
}

/**
 * Code executed by task B
 */
long task_b() {
    printf("Task B starting...\n");
    sleep(10);
    printf("Task B ending...\n");
    return TASK_B_T;
}

/**
 * Code executed by task C
 */
long task_c() {
    printf("Task C starting...\n");
    sleep(15);
    printf("Task C ending...\n");
    return TASK_C_T;
}

/**
 * Code executed by task D
 */
long task_d() {
    printf("Task D starting...\n");
    sleep(20);
    printf("Task D ending...\n");
    return TASK_D_T;
}

/**
 * Initialises a processor structure. This can fail if there is no
 * memory for a tasks list, it's initialisation fails or the mutex
 * cannot be created.
 * @param id the ID of the processor
 * @param p the processor
 * @return if the initialization was successful
 */
bool processor_init(int id, processor *p) {
    if(p == NULL || pthread_mutex_init(&p->lock,NULL) != 0){
        return false;
    }

    pthread_mutex_lock(&p->lock);

    p->id = id;

    if((p->tasks = malloc(sizeof(blocking_q))) == NULL){
        pthread_mutex_unlock(&p->lock);
        pthread_mutex_destroy(&p->lock);
        return false;
    }

    blocking_q_init(p->tasks);

    p->sched_t = p->wait_t = p->work_t = p->real_t = 0.0;

    pthread_mutex_unlock(&p->lock);

    return true;
}

/**
 * Destroy a processor structure
 * @param p ptr to the structure
 */
void processor_destroy(processor *p) {
    pthread_mutex_destroy(&p->lock);

    blocking_q_destroy(p->tasks);
    free(p->tasks);
    p->tasks = NULL;
}

long get_task_time(task_ptr t){
    switch(t->type){
        case 'A':{
            return TASK_A_T;
        }
        case 'B':{
            return TASK_B_T;
        }
        case 'C':{
            return TASK_C_T;
        }
        case 'D':{
            return TASK_D_T;
        }
        default: return 0;
    }
}

void *processor_run(void *v_self) {
    processor *p = (processor *)v_self;
    task_ptr t;
    time_t timer = time(NULL);

    while((t = blocking_q_get(p->tasks))->type != POISON_PILL){
        p->wait_t += (time(NULL) - timer)*1000;
        timer = time(NULL);
        switch (t->type)
        {
            case 'A':{
                task_a();
                break;}
            case 'B':{
                task_b();
                break;}
            case 'C':{
                task_c();
                break;}
            case 'D':{
                task_d();
                break;}
            default:
                break;
        }
        pthread_mutex_lock(&p->lock);
        p->sched_t -= get_task_time(t);
        pthread_mutex_unlock(&p->lock);
        p->work_t += (time(NULL) - timer)*1000;
        timer = time(NULL);
        p->real_t = p->wait_t + p->work_t;

//!!!!! À éliminer lorsque les démos auront free les tâches dans le main
        free(t);
    }
}


void *scheduler(void *v_sched_data) {
    sched_data *data = (sched_data *) v_sched_data;
    blocking_q *q = data->sched_q;
    processor *p = data->processors;

    task_ptr poison = NULL;

    while (!poison) {
        task_ptr t = blocking_q_get(q);
        printf("Received t %c\n", t->type);

        /// ------------------------------------------------------------------
        ///           EXERCICE 2.4 DANS LE BLOC LEXICAL SUIVANT
        /// ------------------------------------------------------------------
        {
            if(POISON_PILL != t->type){
                int i, i_min = 0;
                pthread_mutex_lock(&p[0].lock);
                long t_min = p[0].sched_t,tmp;
                pthread_mutex_unlock(&p[0].lock);

                for(i = 1; i < PROCESSOR_COUNT; i++){
                    pthread_mutex_lock(&p[i].lock);
                    tmp = p[i].sched_t;
                    if(tmp < t_min){
                        i_min = i;
                        t_min = tmp;
                    }
                    pthread_mutex_unlock(&p[i].lock);
                }

                printf("Putting in %d.\n",i_min);
                pthread_mutex_lock(&p[i_min].lock);
                p[i_min].sched_t += get_task_time(t);
                blocking_q_put(p[i_min].tasks,t);
                pthread_mutex_unlock(&p[i_min].lock);
            }
        }
        /// ------------------------------------------------------------------
        ///                NE PAS TOUCHER APRÈS CETTE LIGNE
        /// ------------------------------------------------------------------


        if (POISON_PILL == t->type) {
            poison = t;
        }
    }

    // Stop the processors
    for (int i = 0; i < PROCESSOR_COUNT; ++i) {
        processor *proc = data->processors + i;
        // kill all processors
        blocking_q_put(proc->tasks, poison);
    }

    return NULL;
}


// ATTENTION! TOUT CE QUI EST ENTRE LES BALISES ༽つ۞﹏۞༼つ SERA ENLEVÉ! N'AJOUTEZ PAS D'AUTRES ༽つ۞﹏۞༼つ
// ༽つ۞﹏۞༼つ
/*
 * Entry point to your homework. DO NOT, UNLESS TOLD BY AN INSTRUCTOR, CHANGE ANY CODE IN THIS
 * FUNCTION. DOING SO WILL GIVE YOU THE GRADE 0.
 * @return exit code
 */
int main(int argc, char **argv) {

    if (argc < 2) {
        printf("Missing / Wrong arguments.\n");
        return EXIT_FAILURE;
    }

    // Start threads
    blocking_q *sched_q = malloc(sizeof(blocking_q));

    if (NULL == sched_q || !blocking_q_init(sched_q)) {
        return EXIT_FAILURE;
    }

    pthread_t sched_thread;
    pthread_t processor_threads[PROCESSOR_COUNT];
    processor processors[PROCESSOR_COUNT];

    sched_data data;
    data.sched_q = sched_q;
    data.processors = processors;

    if (0 != pthread_create(&sched_thread, NULL, scheduler, (void *) &data)) {
        return EXIT_FAILURE;
    }

    long start = time(NULL);
    for (int i = 0; i < PROCESSOR_COUNT; ++i) {

        if (!processor_init(i, processors + i)) {
            return EXIT_FAILURE;
        }

        if (0 != pthread_create(processor_threads + i,
                                NULL,
                                processor_run,
                                (void *) (processors + i))) {

            return EXIT_FAILURE;
        }
    }

    char *tasks_and_times = argv[1];

    // Fill the task queue
    unsigned long task_c = strlen(tasks_and_times);
    for (unsigned long i = 0; i < task_c; ++i) {
        char task_type = tasks_and_times[i];

        switch (task_type) {
            case 'A':
            case 'B':
            case 'C':
            case 'D': {
                task_ptr t = (task_ptr) malloc(sizeof(task));
                t->type = task_type;
                t->start = t->end = 0;
                blocking_q_put(sched_q, t);
                break;
            }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                sleep(task_type - '0');
                break;
            default:
                break;
        }
    }

    task_ptr poison_pill_task = malloc(sizeof(task));
    poison_pill_task->type = POISON_PILL;

    blocking_q_put(sched_q, poison_pill_task);

    pthread_join(sched_thread, NULL);

    printf("\n\n");

    for (int i = 0; i < PROCESSOR_COUNT; ++i) {
        pthread_join(processor_threads[i], NULL);

        processor *p = processors + i;
        printf("Processor %d: Real T: %ld Work T: %ld Wait T: %ld\n",
               i,
               p->real_t,
               p->work_t,
               p->wait_t);
    }

    long end = time(NULL);
    long elapsed = end - start;

    printf("Elapsed: %ld\n", elapsed);

    free(poison_pill_task);

    for (int i = 0; i < PROCESSOR_COUNT; ++i) {
        processor *p = processors + i;
        processor_destroy(p);
    }

    free(sched_q);

    return EXIT_SUCCESS;


}
// ༽つ۞﹏۞༼つ
