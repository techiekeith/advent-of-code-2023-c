//
// Created by keith on 12/12/2023.
//

#ifndef ADVENT_OF_CODE_2023_C_ARRAY_H
#define ADVENT_OF_CODE_2023_C_ARRAY_H

enum value_type {
    TYPE_UNDEFINED,
    TYPE_INTEGER,
    TYPE_ARRAY,
};

#define UNDEFINED_VALUE (struct value) { 0 }

struct value {
    enum value_type type;
    bool free_on_destroy;
    union {
        unsigned long long int_value;
        void *data;
        struct array *array_value;
    };
};

struct array {
    size_t capacity;
    size_t num_elements;
    struct value *values;
};

struct array *new_array_with_capacity(size_t capacity);
struct array *new_array();
struct value get_value(struct array *array, size_t index);
void set_value(struct array *array, size_t index, enum value_type type, void *data, bool free_on_destroy);
void set_int_value(struct array *array, size_t index, unsigned long long int_value);
void set_array_value(struct array *array, size_t index, struct array *array_value, bool free_on_destroy);
void set_new_array_value(struct array *array, size_t index);
void destroy_array(struct array *array);

#endif //ADVENT_OF_CODE_2023_C_ARRAY_H
