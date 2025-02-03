/**
 * File: lab.c
 * Author: MTumir
 * Created: 01.22.2025
 * Latest: 02.02.2025
 * 
 * Description: Implements functions defined in lab.h.
 */

#include "lab.h"

list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {

    /* Returns if parameters are not met */
    if (!destroy_data || !compare_to) {     // CR: destroy_data == NULL changed to !destroy_data, compare_to == NULL changed to !compare_to
        return NULL;
    }

    /* Create new list + sentinel node */
    list_t *new_list = (list_t *)malloc(sizeof(list_t));
    if (!new_list) {        // CR: new_list == NULL changed to !new_list
        return NULL;        // CR: now returns early if new_list is NULL
    }
    node_t *sentinel = (node_t *)malloc(sizeof(node_t));
    if (!sentinel) {        // CR: sentinel == NULL changed to !sentinel
        free(new_list);     // CR: now frees new_list if sentinel is NULL
        return NULL;
    }

    /* Set list variables */
    new_list->destroy_data = destroy_data;
    new_list->compare_to = compare_to;
    new_list->size = 0;
    new_list->head = sentinel;

    /* Set sentinel node variables */
    sentinel->data = NULL;
    sentinel->next = sentinel;
    sentinel->prev = sentinel;

    return new_list;

}

void list_destroy(list_t **list) {

    /* Returns if parameters are not met */
    if (!list || !*list) {        // CR: list == NULL changed to !list, added NULL check for *list
        return;
    }

    /* Start from the first non-sentinel node */
    node_t *curr_node = (*list)->head->next;

    /* For each node, destroy it's data, then free the node */
    while (curr_node != (*list)->head) {
        curr_node = curr_node->next;
        (*list)->destroy_data(curr_node->prev->data);
        free(curr_node->prev);
    }

    /* Free the sentinel node and list */
    free(curr_node);
    free(*list);
    *list = NULL;

}

list_t *list_add(list_t *list, void *data) {

    /* Returns if parameters are not met */
    if (!list || !data) {       // CR: list == NULL changed to !list, data == NULL changed to !data
        return list;            // CR: now returns list rather than NULL
    }

    /* Create new node */
    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node) {            // CR: new_node == NULL changed to !new_node
        return list;            // CR: now returns list rather than NULL
    }

    /* Set new node variables */
    new_node->data = data;
    new_node->prev = list->head;
    new_node->next = list->head->next;

    /* Place the node in the list */
    list->head->next->prev = new_node;
    list->head->next = new_node;
    list->size = list->size + 1;

    return list;

}

// CR: Checked for missing free(); not found.
void *list_remove_index(list_t *list, size_t index) {

    /* Returns if parameters are not met */
    if (!list || index > list->size) {      // CR: list == NULL changed to !list
        return NULL;
    }

    /* Start from the first non-sentinel node */
    node_t *curr_node = list->head->next;

    /* Navigate to the desired node */
    for (size_t i = 0; i < index; i ++) {
        curr_node = curr_node->next;
    }

    /* Take the node out of the list */
    curr_node->prev->next = curr_node->next;
    curr_node->next->prev = curr_node->prev;

    /* Take the data, then free the node */
    void *data = curr_node->data;
    free(curr_node);

    list->size = list->size - 1;

    return data;

}

int list_indexof(list_t *list, void *data) {

    /* Returns if parameters are not met */
    if (!list || !data) {       // CR: list == NULL changed to !list, data == NULL changed to !data
        return -1;
    }

    /* Start from the first non-sentinel node */
    node_t *curr_node = list->head->next;

    /* Prepare indexes for traversal */
    int return_index = -1;
    int curr_index = 0;

    /* Traverse the entire list*/
    while (curr_node != list->head) {
        /* If the data matches, return the current index */
        if (!list->compare_to(curr_node->data, data)) {
            return_index = curr_index;
            break;
        }
        /* Else, go to the next node */
        curr_node = curr_node->next;
        curr_index ++;
    }

    return return_index;

}