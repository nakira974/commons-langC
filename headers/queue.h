//
// Created by maxim on 21/02/2024.
//

#ifndef COLLECTIONS_COMMONS_QUEUE_H
#define COLLECTIONS_COMMONS_QUEUE_H
#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

/**
* Data structure definition for a generic queue
* */
typedef LinkedList Queue;

/**
 * Dequeue the first element of the specified queue
 * @param queue The queue to remove the first element in
 * @param value
 * @complexity O(1)
 * @return
 */
int queue_enqueue(Queue *queue, const void* value);

/**
 *
 * @param queue
 * @param value
 *  * @complexity O(1)
 * @return
 */
int queue_dequeue(Queue * queue, void *value);

#ifdef __cplusplus

/***
* Inline function that evaluates the number of elements inside the specified queue
* @return The current element count of the current list
* @complexity O(1)
*/
inline int queue_size(LinkedList *queue){
    return queue->size;
};

/**
 * Inline function that peeks the first element of the queue without unstacking it
 * @param queue Queue to peek the first element in
 * @return The current first element of the queue
 * @complexity O(1)
 */
inline void * queue_peek(Queue  * queue){
    return ((queue)->head == nullptr ? nullptr : (queue)->head->value);
}

/**
 * Creates a default queue structure that can be used for other operations
 * @param queue Reference to the queue to create
 * @param destroy Delegate user function for later destruction of a single element the current queue
 * @complexity O(1)
 */
inline void queue_create(Queue * queue, void( *destroy)(void *value)){
    list_create(queue, destroy);
}

/**
 * Destroy the specified queue, after the call no other further operations will be permit
 * @param queue Reference of the queue to destroy otherwise false
 * @complexity O(n) where n is the number of elements in the current list
 */

inline void queue_destroy(Queue * queue){
    list_destroy(queue);
}
#else

/**
 * Macro that evaluates the queue creation
 * @complexity O(1)
 */
#define queue_create list_create

/**
 * Macro that evaluates queue destruction
 * @complexity O(n) where n is the number of elements in the current list
 */
#define queue_destroy list_destroy

/**
 * Macro that evaluate peek the first element of the queue without unstacking it
 * @param queue Queue to peek the first element in
 * @return The current first element of the queue*
 * @complexity O(1)
 */
#define queue_peek(queue) ((queue)->head == NULL ? NUll : (queue)->head->value)

/***
* Macro that evaluates the number of elements inside the specified queue
* @return The current element count of the current list
* @complexity O(1)
*/
#define queue_size list_size
#endif


#ifdef __cplusplus
}
#endif
#endif //COLLECTIONS_COMMONS_QUEUE_H
