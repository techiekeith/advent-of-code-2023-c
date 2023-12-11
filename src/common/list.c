//
// Created by keith on 11/12/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

struct list *new_list() {
    struct list *list = malloc(sizeof(struct list));
    if (list == NULL) {
        perror("new_list::malloc");
    } else {
        *list = (struct list) { 0 };
    }
    return list;
}

struct list_item *add_item(struct list *list, void *data, size_t size) {
    struct list_item *list_item = malloc(sizeof(struct list_item) + size);
    if (list_item == NULL) {
        perror("add_item::malloc");
    } else {
        *list_item = (struct list_item) { 0 };
        list_item->prev = list->tail;
        list_item->data = (char *)(list_item + 1);
        memcpy(list_item->data, data, size);
        if (list->head == NULL) {
            list->head = list_item;
        }
        if (list->tail != NULL) {
            list->tail->next = list_item;
        }
        list->tail = list_item;
        list->items++;
    }
    return list_item;
}

struct list_item *add_str(struct list *list, char *str_value) {
    add_item(list, str_value, strlen(str_value) + 1);
}

struct list_item *add_int(struct list *list, unsigned long long int_value) {
    add_item(list, &int_value, sizeof(unsigned long long));
}

struct list_item *add_point(struct list *list, size_t row, size_t column) {
    struct point point_value = { row, column };
    add_item(list, &point_value, sizeof(struct point));
}

static void remove_item(struct list *list, struct list_item *item) {
    if (item->prev == NULL) {
        list->head = item->next;
    } else {
        item->prev->next = item->next;
    }
    if (item->next == NULL) {
        list->tail = item->prev;
    } else {
        item->next->prev = item->prev;
    }
    free(item);
    list->items--;
}

void remove_int(struct list *list, unsigned long long int_value) {
    struct list_item *item = list->head;
    while (item != NULL && *item->int_value != int_value) {
        item = item->next;
    }
    if (item != NULL) {
        remove_item(list, item);
        return;
    }
}

void destroy_list(struct list *list) {
    while (list->head != NULL) {
        struct list_item *item = list->head;
        list->head = item->next;
        free(item);
    }
    free(list);
}