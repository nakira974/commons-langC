//
// Created by maxim on 23/02/2024.
//

#include <stdint.h>


#include "set.h"


bool set_match_entries(Set *elements, Set *elements_to_match, Set *matched_elements) {
    Set intersection;
    KeySetEntry *hashSet;
    LinkedElement *current_element, *element_max;
    void *value;
    uint32_t max_size;

    // Initialize the set cover
    set_create(matched_elements, elements_to_match->equals, NULL);

    // Continue until there are non-covering elements and candidates

    while (set_size(elements) > 0 && set_size(elements_to_match) > 0) {
        // Search of the candidate covering the most elements as possible
        max_size = 0;

        for (current_element = list_first(elements_to_match);
             current_element != NULL; current_element = list_next(current_element)) {
            if (!set_intersection(&intersection, &((KeySetEntry *) list_value(current_element))->set, elements))
                return false;

            if (set_size(&intersection) > max_size) {
                element_max = current_element;
                max_size = set_size(&intersection);
            }

            set_destroy(&intersection);
        }
    }

    // A covering isn't possible if there's no intersection

    if (max_size == 0) return false;

    // Insert inside the covering the selected hashset
    hashSet = (KeySetEntry *) list_value(element_max);

    if (!set_add(matched_elements, hashSet)) return false;

    // Remove each current_element covered from the uncovered-elements set
    for (current_element = list_first(&((KeySetEntry *) list_value(element_max))->set);
         current_element != NULL; current_element = list_next(current_element)) {
        value = list_value(current_element);

        if (set_remove(elements, (void **) &value) && elements->destroy != NULL) elements->destroy(value);
    }

    // Remove the hashset from the hashset candidates

    if (!set_remove(elements_to_match, (void **) &hashSet)) return false;

    // No covering if there's still non-covered elements
    if (set_size(elements) > 0) return false;

    return true;
}

void set_create(Set *set, bool (*equals)(const void *left, const void *right), void (*destroy)(void *value)) {
    list_create(set, destroy);
    set->equals = equals;
}

bool set_add(Set *set, const void *value) {
    // No duplicated values
    if (set_isMember(set, value)) return false;
    // Add the value at the end
    return list_add(set, list_last(set), value);
}

bool set_remove(Set *set, void **value) {
    LinkedElement *current_element, *element_to_remove;

    // Search for a value to remove

    for (current_element = list_first(set); current_element != NULL; current_element = list_next(current_element)) {
        if (set->equals(*value, list_value(current_element))) {
            element_to_remove = current_element;
            break;
        }
    }

    // Element not found case

    if (current_element == NULL) return false;

    if (list_size(set) == 1) return list_remove(set, NULL, value);
    else return list_remove(set, element_to_remove, value);
}

bool set_union(Set *union_result, const Set *left, const Set *right) {
    LinkedElement *current_element;
    void *value;

    // Create the union set
    set_create(union_result, left->equals, NULL);

    // Insertion of left set elements
    for (current_element = list_first(left); current_element != NULL; current_element = list_next(current_element)) {
        value = list_value(current_element);

        if (!list_add(union_result, list_last(union_result), value)) {
            set_destroy(union_result);
            return false;
        }
    }

    // Insertion of right set elements
    for (current_element = list_first(right); current_element != NULL; current_element = list_next(current_element)) {
        if (set_isMember(left, list_value(current_element))) continue;
        else {
            value = list_value(current_element);
            if (!list_add(union_result, list_last(union_result), value)) {
                set_destroy(union_result);
                return false;
            }
        }
    }
    return true;
}

bool set_intersection(Set *intersection_result, const Set *left, const Set *right) {
    LinkedElement *current_element;
    void *value;

    // Create the intersection Set

    set_create(intersection_result, left->equals, NULL);

    // intersection of elements in left and right set

    for (current_element = list_first(left); current_element != NULL; current_element = list_next(current_element)) {
        // If the current left element is in the right Set
        if (set_isMember(right, list_value(current_element))) {
            value = list_value(current_element);
            if (!list_add(intersection_result, list_last(intersection_result), value)) {
                set_destroy(intersection_result);
                return false;
            }
        }
    }
    return true;
}

bool set_difference(Set *difference_result, const Set *left, const Set *right) {
    LinkedElement *current_element;
    void *value;

    // Creation of the difference Set
    set_create(difference_result, left->equals, NULL);

    // Insert elements of left non present in right
    for (current_element = list_first(left); current_element != NULL; current_element = list_next(current_element)) {
        // If the current left value is not in the right set
        if (!set_isMember(right, list_value(current_element))) {
            value = list_value(current_element);
            if (!list_add(difference_result, list_last(difference_result), value)) {
                set_destroy(difference_result);
                return false;
            }
        }
    }
    return true;
}

bool set_isMember(const Set *set, const void *value) {
    LinkedElement *current_element;

    // Determine if the value is in set

    for (current_element = list_first(set); current_element != NULL; current_element = list_next(current_element)) {
        // If any equals occur, then return true
        if (set->equals(value, list_value(current_element))) {
            return true;
        }
    }
    return false;
}

bool set_isSubset(const Set *left, const Set *right) {
    LinkedElement *current_element;

    // Quick test to eliminate some usual cases
    if (set_size(left) > set_size(right)) return false;
    // Determine if left is a subset of right
    for (current_element = list_first(left); current_element != NULL; current_element = list_next(current_element)) {
        // Validate one by one left elements in right set independently of their order
        if (!set_isMember(right, list_value(current_element))) return false;
    }
    return true;
}

bool set_equals(const Set *left, const Set *right) {
    LinkedElement *current_element;
    // Quick test to eliminate usual cases

    if (set_size(left) != set_size(right)) return false;

    // Determine if left and right sets are equal
    for (current_element = list_first(left); current_element != NULL; current_element = list_next(current_element)) {
        // If there is one element of left not present in right, sets are not equal
        if (!set_isMember(right, list_value(current_element))) return false;
    }

    return true;
}