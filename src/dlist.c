//
// Created by maxim on 20/02/2024.
//

#include "dlist.h"

void dlist_create(DLinkedList * list, void( * destroy)(void * value)){
    // Default values
    list->size=0;
    list->destroy=destroy;
    list->tail= nullptr;
    list->head= nullptr;
}


void dlist_destroy(DLinkedList * list){
    void* value = nullptr;
    // While is list is not empty custom user func destroy values
    while(dlist_size(list)>0){
        if(dlist_remove(list, dlist_last(list), (void **) &value) && list->destroy != nullptr){
            list->destroy(value);
        }
    }

    // Ensure the list is cleaned up at the end
    memset(list, 0,sizeof (DLinkedList));
}

bool dlist_add(DLinkedList *list, DLinkedElement * element, const void *value){
    DLinkedElement*  new_element = nullptr;
    // Reject null elements except if list is empty
    if(element == nullptr && dlist_size(list) != 0) return false;

    // Allocate a new memory space for the element
    if((new_element = (DLinkedElement*) malloc(sizeof (DLinkedElement))) == nullptr)
        return false;

    new_element->value = (void*) value;
    if(dlist_size(list) == 0){
        // Empty list case
        list->head=new_element;
        list->head->previous= nullptr;
        list->head->next= nullptr;
        list->tail= nullptr;
    }else{
        // Non-empty list case
        new_element ->next = element->next;
        new_element->previous=element;
        if(element->next == nullptr) list->tail = new_element;
        else element->next=new_element;
    }

    list->size++;
    return true;
}

bool dlist_add_before(DLinkedList *list, DLinkedElement * element, const void *value){
    DLinkedElement*  new_element = nullptr;
    // Reject null elements except if list is empty
    if(element == nullptr && dlist_size(list) != 0) return false;

    // Allocate a new memory space for the element
    if((new_element = (DLinkedElement*) malloc(sizeof (DLinkedElement))) == nullptr)
        return false;

    new_element->value = (void*) value;
    if(dlist_size(list)>0){
        // Empty list case
        list->head = new_element;
        list->head->previous = nullptr;
        list->head->next = nullptr;
        list->tail = new_element;
    }else{
        // Non-empty list case

        // The new element is just before the target element
        new_element->next=element;
        // The new element is inserted between the target and its current previous
        new_element->previous = element->previous;

        // If we're on top of list then the new element become the head
        if(element->previous == nullptr)
            list->head = new_element;
        // else before replacing the previous element we need to update the current previous element next reference to the new created element
        else element->previous->next = new_element;

        // finally replace the previous element
        element->previous = new_element;
    }

    list->size++;

    return true;
}

bool dlist_remove(DLinkedList * list, DLinkedElement * element, void **value){
    // Do not authorize a null element or in an empty list
    if(dlist_size(list) == 0 || element == nullptr){
        return false;
    }

    // Remove the element from the list
    *value = element->value;
    if(element == list->head){
        // The list become after deletion empty case
        list->head=element->next;
        if(list->head== nullptr)
            list->tail= nullptr;
        else
            element->next->previous= nullptr;
    }else{
        // The list does not become empty after deletion case
        element->previous->next = element->previous;

        if(element->next == nullptr)
            list->tail = element->previous;
        else element->next->previous=element->previous;
    }

    free(element);

    list->size--;
    return true;
}