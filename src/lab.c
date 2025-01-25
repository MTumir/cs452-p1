/**
 * File: lab.c
 * Author: MTumir
 * Created: 01.22.2025
 * Latest: 01.24.2025
 * 
 * Description: Implements functions defined in lab.h.
 */

#include "lab.h"

list_t *list_init(void (*destroy_data)(void *), int (*compare_to)(const void *, const void *)) {

    if (destroy_data == NULL || compare_to == NULL) {
        return NULL;
    }

    list_t *new_list = (list_t *)malloc(sizeof(list_t));
    node_t *sentinel = (node_t *)malloc(sizeof(node_t));
    if (new_list == NULL || sentinel == NULL) {
        return NULL;
    }

    new_list->destroy_data = destroy_data;
    new_list->compare_to = compare_to;
    new_list->size = 0;
    new_list->head = sentinel;

    sentinel->data = NULL;
    sentinel->next = sentinel;
    sentinel->prev = sentinel;

    return new_list;

}

void list_destroy(list_t **list) {

    if (list == NULL) {
        return;
    }

    node_t *curr_node = (*list)->head->next;

    while (curr_node != (*list)->head) {
        curr_node = curr_node->next;
        (*list)->destroy_data(curr_node->prev->data);
        free(curr_node->prev);
    }

    free(curr_node);
    free(*list);
    *list = NULL;

}

list_t *list_add(list_t *list, void *data) {

    if (list == NULL || data == NULL) {
        return NULL;
    }

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = data;

    new_node->prev = list->head;
    new_node->next = list->head->next;
    list->head->next->prev = new_node;
    list->head->next = new_node;
    list->size = list->size + 1;

    return list;

}

void *list_remove_index(list_t *list, size_t index) {

    if (list == NULL || index > list->size) {
        return NULL;
    }

    node_t *curr_node = list->head->next;

    for (size_t i = 0; i < index; i ++) {
        curr_node = curr_node->next;
    }

    curr_node->prev->next = curr_node->next;
    curr_node->next->prev = curr_node->prev;

    void *data = curr_node->data;
    free(curr_node);

    list->size = list->size - 1;

    return data;

}

int list_indexof(list_t *list, void *data) {

    if (list == NULL || data == NULL) {
        return -1;
    }

    node_t *curr_node = list->head->next;

    int return_index = -1;
    int curr_index = 0;
    while (curr_node != list->head) {
        if (!list->compare_to(curr_node->data, data)) {
            return_index = curr_index;
            break;
        }
        curr_node = curr_node->next;
        curr_index ++;
    }

    return return_index;

}