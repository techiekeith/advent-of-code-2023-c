//
// Created by keith on 12/12/2023.
//

#include <stdbool.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "array.h"
#include "file.h"

static struct array *cached_results;

static struct array *current_array_of_numbers;

static char *current_pattern;

static char *buffer;

static struct value get_cached_result(size_t idx, size_t max, size_t offset) {
    size_t key = idx << 16 | max << 8 | offset; // very crude but it'll do!
    return get_value(cached_results, key);
}

static void set_cached_result(size_t idx, size_t max, size_t offset, unsigned long long result) {
    size_t key = idx << 16 | max << 8 | offset; // very crude but it'll do!
    set_int_value(cached_results, key, result);
}

static bool may_contain(const char *pattern, const char *substring, size_t offset) {
    for (int i = 0; substring[i] != '\0'; i++) {
        if (pattern[offset + i] != substring[i] && pattern[offset + i] != '?') {
            return false;
        }
    }
    return true;
}

static unsigned long long recurse(size_t idx, size_t max, size_t offset) {
    struct value cached_result = get_cached_result(idx, max, offset);
    if (cached_result.type == TYPE_INTEGER) {
        return cached_result.int_value;
    }

    unsigned long long sum = 0;
    unsigned long long to_match = current_array_of_numbers->values[idx].int_value;
    bool has_more = idx < current_array_of_numbers->num_elements - 1;

    for (size_t i = 0; i <= max; i++) {
        char *p = buffer;
        for (size_t n = 0; n < i; n++) *p++ = '.';
        for (size_t n = 0; n < to_match; n++) *p++ = '#';
        if (has_more) *p++ = '.';
        *p = '\0';
        size_t buffer_len = p - buffer;

        if (may_contain(current_pattern, buffer, offset)) {
            if (has_more) {
                sum += recurse(idx + 1, max - i, offset + buffer_len);
            } else {
                p = buffer;
                for (int n = 0; n < max - i; n++) *p++ = '.';
                *p = '\0';
                if (may_contain(current_pattern, buffer, offset + buffer_len)) {
                    sum++;
                }
            }
        }
    }

    set_cached_result(idx, max, offset, sum);
    return sum;
}

static struct array *create_array_of_numbers(char *string, char separator) {
    struct array *array_of_numbers = new_array();
    if (array_of_numbers == NULL) {
        perror("create_array_of_numbers::new_array");
        return NULL;
    }
    size_t string_size = strlen(string) + 1;
    char *string_copy = malloc(string_size);
    if (string_copy == NULL) {
        perror("create_array_of_numbers::malloc");
        destroy_array(array_of_numbers);
        return NULL;
    }
    memcpy(string_copy, string, string_size);
    char *current_number = string_copy, *next_number;
    size_t idx = 0;
    do {
        next_number = strchr(current_number, separator);
        if (next_number != NULL) {
            *next_number++ = '\0';
        }
        set_int_value(array_of_numbers, idx++, atoll(current_number));
        current_number = next_number;
    } while (next_number != NULL);
    free(string_copy);
    return array_of_numbers;
}

static char *create_string_with_collapsed_dots(char *source) {
    char *new_string = malloc(strlen(source) + 1);
    if (new_string == NULL) {
        perror("create_string_with_collapsed_dots::malloc");
        return 0;
    }
    char *p = source, *q = new_string;
    while (*p == '.') p++;
    bool has_space = false;
    while (*p) {
        char c = *p++;
        if (c == '.') {
            has_space = true;
        } else {
            if (has_space) {
                *q++ = '.';
                has_space = false;
            }
            *q++ = c;
        }
    }
    *q = '\0';
    return new_string;
}

static unsigned long long get_matching_permutations(char *chars, char *numbers) {
    current_array_of_numbers = create_array_of_numbers(numbers, ',');
    if (current_array_of_numbers == NULL) {
        perror("get_matching_permutations::create_list_of_numbers (current_array_of_numbers)");
        return 0;
    }

    current_pattern = create_string_with_collapsed_dots(chars);
    if (current_pattern == NULL) {
        perror("get_matching_permutations::create_string_with_collapsed_dots (current_pattern)");
        return 0;
    }

    size_t min_length = current_array_of_numbers->num_elements - 1;
    for (size_t idx = 0; idx < current_array_of_numbers->num_elements; idx++) {
        min_length += current_array_of_numbers->values[idx].int_value;
    }
    size_t diff = strlen(current_pattern) - min_length;

    cached_results = new_array();
    if (cached_results == NULL) {
        perror("get_matching_permutations::new_array (cached_results)");
        return 0;
    }

    buffer = malloc(strlen(current_pattern) + 1);
    if (buffer == NULL) {
        perror("get_matching_permutations::malloc (buffer)");
        return 0;
    }

    unsigned long long result = recurse(0, diff, 0);

    free(buffer);
    destroy_array(cached_results);
    free(current_pattern);
    destroy_array(current_array_of_numbers);

    return result;
}

static unsigned long long get_matching_permutations_part_one(char *line, size_t idx) {
    unsigned long long result = 0;
    char *split = strchr(line, ' ');
    if (split != NULL) {
        *split++ = '\0';
        result = get_matching_permutations(line, split);
    }
    return result;
}

static char *repeat_string_with_separator(char *source, int repeat, char separator) {
    size_t new_string_size = (strlen(source) + 1) * repeat;
    char *new_string = malloc(new_string_size);
    if (new_string == NULL) {
        perror("repeat_string_with_separator::malloc");
        return NULL;
    }
    char *target = new_string;
    for (int i = 0; i < repeat; i++) {
        strcpy(target, source);
        target += strlen(source);
        *target++ = separator;
    }
    *--target = '\0';
    return new_string;
}

static unsigned long long get_matching_permutations_part_two(char *line, size_t idx) {
    unsigned long long result = 0;
    printf("Index: %zu\n", idx);
    char *split = strchr(line, ' ');
    if (split != NULL) {
        *split++ = '\0';
        char *expanded_line = repeat_string_with_separator(line, 5, '?');
        char *expanded_numbers = repeat_string_with_separator(split, 5, ',');
        result = get_matching_permutations(expanded_line, expanded_numbers);
        free(expanded_numbers);
        free(expanded_line);
    }
    return result;
}

static unsigned long long sum_up(struct list *lines, unsigned long long function(char *, size_t)) {
    unsigned long long sum = 0;
    size_t idx = 0;
    for (struct list_item *item = lines->head; item != NULL; item = item->next) {
        sum += function(item->str_value, idx++);
    }
    return sum;
}

void day12_test_part1() {
    struct list *lines = load_file("data/day12/test.txt");
    printf("Test data part 1: %llu\n", sum_up(lines, get_matching_permutations_part_one));
}

void day12_test_part2() {
    struct list *lines = load_file("data/day12/test.txt");
    printf("Test data part 2: %llu\n", sum_up(lines, get_matching_permutations_part_two));
}

unsigned long long day12_part1() {
    struct list *lines = load_file("data/day12/input.txt");
    return sum_up(lines, get_matching_permutations_part_one);
}

unsigned long long day12_part2() {
    struct list *lines = load_file("data/day12/input.txt");
    return sum_up(lines, get_matching_permutations_part_two);
}
