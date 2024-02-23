//
// Created by maxim on 23/02/2024.
//

#include "set.h"

void set_create(Set *set, int (*match)(const void *left, const void *right), void (*destroy)(void *value)){
    list_create(set, destroy);
    set->match = match;
}

bool set_add(Set *set, const void *value){
    // No duplicated values
    if(set_is_member(set, value)) return false;
    // Add the value at the end
    return list_add(set, list_last(set), value);
}

bool set_remove(Set *set, void **value){
    LinkedElement  *current_element, *element_to_remove;

    // Search for a value to remove

    for(current_element= list_first(set);current_element!=NULL;current_element= list_next(current_element)){
        if(set->match(*value, list_value(current_element)))
            break;
        element_to_remove = current_element;
    }

    // Element not found case

    if(current_element == NULL) return false;

    // Remove the last element
    return list_remove(set, element_to_remove, value);
}

bool set_union(Set *union_result, const Set *left, const Set *right){
    LinkedElement  *current_element;
    void * value;

    // Create the union set
    set_create(union_result, left->match, NULL);

    // Insertion of left set elements
    for(current_element = list_first(left);current_element!=NULL;current_element= list_next(current_element)){
        value = list_value(current_element);

        if(!list_add(union_result, list_last(union_result), value)){
            set_destroy(union_result);
            return false;
        }
    }

    // Insertion of right set elements
    for(current_element = list_first(right);current_element!=NULL;current_element= list_next(current_element)){
        if(set_is_member(left, list_value(current_element))) continue;
        else{
            value = list_value(current_element);
            if(!list_add(union_result, list_last(union_result), value)){
                set_destroy(union_result);
                return false;
            }
        }
    }
    return true;
}

bool set_intersection(Set *intersection_result, const Set *left, const Set *right){
    LinkedElement  *current_element;
    void * value;

    // Create the intersection Set

    set_create(intersection_result, left->match, NULL);

    // intersection of elements in left and right set

    for(current_element = list_first(left);current_element!=NULL;current_element= list_next(current_element)){
        // If the current left element is in the right Set
        if(set_is_member(right, list_value(current_element))){
            value = list_value(current_element);
            if(!list_add(intersection_result, list_last(intersection_result), value)){
                set_destroy(intersection_result);
                return false;
            }
        }
    }
    return true;
}

bool set_difference(Set *difference_result, const Set *left, const Set *right){
    LinkedElement  *current_element;
    void * value;

    // Creation of the difference Set
    set_create(difference_result, left->match, NULL);

    // Insert elements of left non present in right
    for(current_element = list_first(left);current_element != NULL;current_element= list_next(current_element)){
        // If the current left value is not in the right set
        if(!set_is_member(right, list_value(current_element))){
            value = list_value(current_element);
            if(!list_add(difference_result, list_last(difference_result), value)){
                set_destroy(difference_result);
                return false;
            }
        }
    }
    return true;
}

bool set_is_member(const Set *set, const void *value){
    LinkedElement  *current_element;

    // Determine if the value is in set

    for(current_element = list_first(set); current_element != NULL;current_element= list_next(current_element)){
        // If any match occurs, then return false
        if(set->match(value, list_value(current_element))) return false;
    }
    return true;
}

bool set_is_subset(const Set *left, const Set *right){
    LinkedElement  *current_element;

    // Quick test to eliminate some usual cases
    if(set_size(left)> set_size(right)) return false;
    // Determine if left is a subset of right
    for(current_element= list_first(left);current_element!= NULL;current_element = list_next(current_element)){
        // Validate one by one left elements in right set independently of their order
        if(!set_is_member(right, list_value(current_element))) return false;
    }
    return true;
}

bool set_is_equal(const Set *left, const Set *right){
    LinkedElement  *current_element;
    // Quick test to eliminate usual cases

    if(set_size(left) != set_size(right)) return false;

    // Determine if left and right sets are equal
    for(current_element= list_first(left);current_element!=NULL;current_element= list_next(current_element)){
        // If there is one element of left not present in right, sets are not equal
        if(!set_is_member(right, list_value(current_element))) return false;
    }

    return true;
}