//
// Created by keith on 11/12/2023.
//

#ifndef ADVENT_OF_CODE_2023_C_LIST_H
#define ADVENT_OF_CODE_2023_C_LIST_H

#include <stddef.h>

struct point {
    size_t row;
    size_t column;
};

struct list_item {
    struct list_item *prev;
    struct list_item *next;
    union {
        void *data;
        char *str_value;
        struct point *point_value;
        unsigned long long *int_value;
    };
};

struct list {
    size_t items;
    struct list_item *head;
    struct list_item *tail;
};

struct list *new_list();
struct list_item *add_item(struct list *list, void *data, size_t size);
struct list_item *add_str(struct list *list, char *str_value);
struct list_item *add_int(struct list *list, unsigned long long int_value);
struct list_item *add_point(struct list *list, size_t row, size_t column);
void remove_int(struct list *list, unsigned long long int_value);
void destroy_list(struct list *list);

#endif //ADVENT_OF_CODE_2023_C_LIST_H
