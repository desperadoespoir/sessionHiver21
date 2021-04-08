#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <pthread.h>
#include "blocking_q.h"

//#pragma clang diagnostic push
//#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#define TODO printf("TODO");

#define TASK_A_T (5 * 1000)
#define TASK_B_T (10 * 1000)
#define TASK_C_T (15 * 1000)
#define TASK_D_T (20 * 1000)

#define PROCESSOR_COUNT 4

#define POISON_PILL 'K'

unsigned long task_c = 50000;

blocking_q *sched_q;

void *putter(void *nothing){
    // Fill the task queue
    char task_type;
    for (unsigned long i = 0; i < task_c; ++i) {
        task_type = 'A' + (char)(((float) rand())/((float) RAND_MAX)*4.0);

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
            default:
                break;
        }
    }

    return NULL;
}

void *taker(void *nothing){
    task_ptr t;
    for (unsigned long i = 0; i < task_c; ++i) {
        t = blocking_q_get(sched_q);
        free(t);
    }
    
    return NULL;
}

/**
 * Entry point to your homework. DO NOT, UNLESS TOLD BY AN INSTRUCTOR, CHANGE ANY CODE IN THIS
 * FUNCTION. DOING SO WILL GIVE YOU THE GRADE 0.
 * @return exit code
 */
int main() {
    // Start threads
    sched_q = malloc(sizeof(blocking_q));

    if (NULL == sched_q || !blocking_q_init(sched_q)) {
        return EXIT_FAILURE;
    }

    pthread_t test_put1, test_put2, test_take1, test_take2;

    if (0 != pthread_create(&test_put1, NULL, putter, NULL)) {
        return EXIT_FAILURE;
    }
    if (0 != pthread_create(&test_take1, NULL, taker, NULL)) {
        return EXIT_FAILURE;
    }
    if (0 != pthread_create(&test_put2, NULL, putter, NULL)) {
        return EXIT_FAILURE;
    }
    if (0 != pthread_create(&test_take2, NULL, taker, NULL)) {
        return EXIT_FAILURE;
    }

    pthread_join(test_put1,NULL);
    pthread_join(test_put2,NULL);
    pthread_join(test_take1,NULL);
    pthread_join(test_take2,NULL);

    printf("There remains %ld items in the queue.\n",sched_q->sz);

    blocking_q_destroy(sched_q);

    return EXIT_SUCCESS;
}
