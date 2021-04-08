#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "blocking_q.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#define TODO printf("TODO!\n");

/**
 * Internal function to blocking_q. Takes an element
 * in the queue. This functions assumes the following
 * preconditions:
 *  - The thread has safe access to the queue
 *  - The queue is NOT empty
 * Also update the size.
 * @param q the queue
 * @return an element
 */
task_ptr __blocking_q_take(blocking_q *q) { // NOLINT(bugprone-reserved-identifier)
    blocking_q_node *temp = q->first;

    //Réassignation du prochain
    q->first = temp->next;
    q->sz--; //Ajustement de la taille

    task_ptr ret = temp->data;

    free(temp);

    return ret;
}

/**
 * Create a blocking queue. Initializes the synchronisation primitives
 * and
 * @param q the queue
 * @return if init was successful.
 */
bool blocking_q_init(blocking_q *q) {
    int code = 0; //Si c'est 0 on n'a pas d'erreurs

    //On commence par initialiser le locker et la condition
    code += pthread_mutex_init(&q->lock, NULL);
    code += pthread_cond_init(&q->cond, NULL);

    //En cas d'erreur d'initialisation des mutex
    if (code != 0) {
        return false;
    }

    q->first = NULL;
    q->sz = 0; //Initialisation à 0 de la taille

    return true;
}

/**
 * Destroy a blocking queue. Removes the allocations of the data
 * and destroys the sync. primitives.
 * @param q ptr to the blocking queue
 */
void blocking_q_destroy(blocking_q *q) {

    pthread_mutex_lock(&q->lock);

    blocking_q_node *temp;
    while (q->sz != 0) {
        temp = q->first;
        q->first = q->first->next;
        q->sz--;

        free(temp);
    }

    pthread_mutex_unlock(&q->lock);

    pthread_mutex_destroy(&q->lock);
    pthread_cond_destroy(&q->cond);

}

/**
 * Put a task in the blocking queue. This task can fail if no
 * memory is available to allocate a new entry in the queue
 * @param q the queue
 * @param data the data description to put inside the queue
 * @returns if the data was put correctly inside the queue.
 */
bool blocking_q_put(blocking_q *q, task_ptr data) {
    blocking_q_node *node = NULL;
    if ((node = malloc(sizeof(blocking_q_node))) == NULL) {
        return false;
    }

    node->data = data;
    node->next = NULL;

    pthread_mutex_lock(&q->lock);

    if (q->sz == 0) {
        q->first = node;
        pthread_cond_signal(&q->cond);
    } else {
        blocking_q_node *temp = q->first;

        while (temp->next != NULL) {
            temp = temp->next;
        }

        temp->next = node;
    }

    q->sz++;

    pthread_mutex_unlock(&q->lock);

    //Techniquement "This task can fail if no memory is available to allocate a new entry in the queue"
    //, mais je ne suis pas sur comment ca pourrait arriver?
    // Il faudrait retrourner faux dans ce cas
    return true;
}

/**
 * Get an element in the blocking queue. If the queue is empty,
 * the current thread is put to sleep until an element is added
 * to the queue.
 * @param q the blocking queue
 * @return the element
 */
task_ptr blocking_q_get(blocking_q *q) {
    pthread_mutex_lock(&q->lock);

    //Il n'y a rien dans la queue, on attend
    if (q->sz == 0) {
        pthread_cond_wait(&q->cond, &q->lock);
    }

    task_ptr task = __blocking_q_take(q);
    pthread_mutex_unlock(&q->lock);

    return task;
}

/**
 * Drain as many elements as possible into the area allowed
 * by the pointer. This function does not block.
 * @param q the queue
 * @param data the pointer where to store the data
 * @param sz the maximum area available in the buffer
 * @return the number of entries written.
 */
size_t blocking_q_drain(blocking_q *q, task_ptr *data, size_t sz) {
    pthread_mutex_lock(&q->lock);
    size_t n = 0;

    while (q->sz != 0 && n < sz) {
        data[n] = __blocking_q_take(q);
        n++;
    }

    pthread_mutex_unlock(&q->lock);

    return n;
}

/**
 * Drain at least min elements in the buffer. This function
 * might block if there are not enough elements to drain.
 * @param q the queue
 * @param data the pointer where to store the data
 * @param sz the maximum area available in the buffer
 * @param min the minimum amounts of elements to drain (must be less than sz)
 * @return the number of elements written
 */
size_t blocking_q_drain_at_least(blocking_q *q, task_ptr *data, size_t sz, size_t min) {
    size_t n = 0;

    while (n < min) {
        data[n] = blocking_q_get(q);
        n++;
    }

    pthread_mutex_lock(&q->lock);

    //Peut-etre devrait etre n < sz/sizeof(task_ptr) au lieu de n < sz ?
    while (q->sz != 0 && n < sz) {
        data[n] = __blocking_q_take(q);
        n++;
    }

    pthread_mutex_unlock(&q->lock);

    return n;
}

/**
 * Check the first element in the queue. This will allocate storage for a copy
 * of the character. If the allocation fails, this function returns false.
 * @param q the queue
 * @param c pointer to a pointer where an allocated char will be stored
 * @return if there is an element allocated in the pointer
 */
bool blocking_q_peek(blocking_q *q, task **c) {
    pthread_mutex_lock(&q->lock);

    if (q->sz > 0) {
        task_ptr task = q->first->data;
        task_ptr copy = malloc(sizeof(task_ptr));
        copy->type = task->type;
        *c = copy;
        return true;
    }

    return false;
}
