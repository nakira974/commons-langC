//
// Created by maxim on 20/02/2024.
//

#ifndef COLLECTIONS_COMMONS_LIST_H
#define COLLECTIONS_COMMONS_LIST_H
#include "stdlib.h"
#include "node.h"

/* Data structure definition for generic linked list type*/
typedef struct List{
    int size;
    int (*match)(const void *val1, const void *val2);
    void (*destroy)(void *value);

    Node  *head;
    Node *tail;
} List;

/* ----- PUBLIC DEFINITIONS ----- */

/**
 * Creates a linked list that can be used for other operations
 *
 * @param list Reference of the list to create
 * @param destroy Delegate function for later destruction of the current list
 * @complexity O(1)
 * @see void destroy_list(List * list)
 */
void create_list(List * list, void( * destroy)(void * value));

/**
 * Destroy the specified list, after the call no other further operations will be permit
 * @param list Reference of the list to destroy otherwise false
 * @complexity O(n) where n is the number of elements in the current list
 */
void destroy_list(List * list);


/**
 * Insert a new element just after element parameter
 * @param list Reference of the list to add an element
 * @param element Reference element of the current list to add after
 * @param value A generic data to add after the element parameter
 * @complexity O(1)
 * @return true if the element was added to the current list, otherwise false
 *
 */
bool add(List *list, Node* element, const void *value);

/**
 * Remove from an element from the current list, then returns a pointer on the value of the deleted element
 * @param list Reference of the list to remove an element
 * @param element Element of the list to be removed
 * @param value Output pointer on the value of the deleted list element reference
 * @complexity O(1)
 * @return true if the element was correctly removed, otherwise false
 */
bool remove(List * list, Node * element, void **value);

/***
 * Macro that evaluates the number of elements inside the specified list
 * @return The current element count of the current list
 * @complexity O(1)
 */
#define size(list) ((list)->size)

/***
 * Macro that evaluates the first element of the specified list
 * @return The first element of the current list
 * @complexity O(1)
 */
#define first(list) ((list)->head)

/***
 * Macro that evaluates the last element of the specified list
 * @return The last element of the current list
 * @complexity O(1)
 */
#define last(list) ((list)->tail)

/***
 * Macro that evaluates if the specified element is the first element of the specified list
 * @return true if the element is the first of the current list, otherwise false
 * @complexity O(1)
 */
#define is_first(list, element) ((element) == (list)->head ? true : false )

/***
 * Macro that evaluates if the specified element is the last element of the specified list
 * @return true if the element is the last of the current list, otherwise false
 * @complexity O(1)
 */
#define is_last(list, element) ((element) == (list)->tail ? true : false )

/***
 * Macro that evaluates the value of a list element
 * @return The value stored inside a list element
 * @complexity O(1)
 */
#define value(element) ((element)->value)


/***
 * Macro that evaluates the next element of the current list element
 * @return The reference to the next element of the current list element
 * @complexity O(1)
 */
#define next(element) ((element)->next)

#endif //COLLECTIONS_COMMONS_LIST_H
