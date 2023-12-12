//
// Created by keith on 12/12/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "array.h"

#define PAGE_SIZE 64

static void resize_array(struct array *array, size_t capacity) {
    size_t actual_size = (((capacity + 1) / PAGE_SIZE) + 1) * PAGE_SIZE;
    size_t size = sizeof(struct value) * actual_size;
    void *new_data = malloc(size);
    if (new_data == NULL) {
        perror("resize_array::malloc");
        return;
    }
    memset(new_data, 0, size);
    if (array->values != NULL) {
        size_t elements_to_copy = actual_size < array->capacity ? actual_size : array->capacity;
        memcpy(new_data, array->values, sizeof(struct value) * elements_to_copy);
        free(array->values);
    }
    array->capacity = actual_size;
    array->values = new_data;
}

struct array *new_array_with_capacity(size_t capacity) {
    struct array *array = malloc(sizeof(struct array));
    if (array == NULL) {
        perror("new_array::malloc");
    } else {
        *array = (struct array) { 0 };
        resize_array(array, capacity);
    }
    return array;
}

struct array *new_array() {
    return new_array_with_capacity(0);
}

struct value get_value(struct array *array, size_t index) {
    if (index >= array->capacity) {
        resize_array(array, index);
    }
    return array->values[index];
}

void unset_value(struct array *array, size_t index) {
    if (index >= array->capacity) {
        resize_array(array, index);
    }
    if (array->values[index].free_on_destroy) {
        if (array->values[index].type == TYPE_ARRAY) {
            destroy_array(array->values[index].array_value);
        } else {
            free(array->values[index].data);
        }
    }
    array->values[index] = (struct value) { 0 };
}

void set_value(struct array *array, size_t index, enum value_type type, void *data, bool free_on_destroy) {
    unset_value(array, index);
    array->values[index].type = type;
    array->values[index].free_on_destroy = free_on_destroy;
    array->values[index].data = data;
    if (index >= array->num_elements) {
        array->num_elements = index + 1;
    }
}

void set_int_value(struct array *array, size_t index, unsigned long long int_value) {
    unset_value(array, index);
    array->values[index].type = TYPE_INTEGER;
    array->values[index].int_value = int_value;
    if (index >= array->num_elements) {
        array->num_elements = index + 1;
    }
}

void set_array_value(struct array *array, size_t index, struct array *array_value, bool free_on_destroy) {
    set_value(array, index, TYPE_ARRAY, array_value, free_on_destroy);
}

void set_new_array_value(struct array *array, size_t index) {
    set_array_value(array, index, new_array(), true);
}

void destroy_array(struct array *array) {
    if (array->values != NULL) {
        for (int i = 0; i < array->num_elements; i++) {
            if (array->values[i].free_on_destroy) {
                if (array->values[i].type == TYPE_ARRAY) {
                    destroy_array(array->values[i].array_value);
                } else {
                    free(array->values[i].data);
                }
            }
        }
        free(array->values);
    }
    free(array);
}
