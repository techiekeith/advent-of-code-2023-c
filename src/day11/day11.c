//
// Created by keith on 11/12/2023.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file.h"

struct grid {
    size_t rows;
    size_t columns;
    struct list *empty_rows;
    struct list *empty_columns;
    struct list *galaxies;
    struct list *expanded_galaxies;
    char *data;
};

static void populate_grid(struct grid *grid, struct list *list) {
    struct list_item *item = list->head;
    for (int row = 0; row < grid->rows; row++) {
        memcpy(grid->data + row * grid->columns, item->str_value, grid->columns);
        item = item->next;
    }
}

static struct grid *new_grid(size_t rows, size_t columns) {
    struct grid *grid = malloc(sizeof(struct grid) + rows * columns);
    *grid = (struct grid) { rows, columns, .data = (char *)(grid + 1) };
    return grid;
}

static void find_empty_rows_and_columns(struct grid *grid) {
    grid->empty_rows = new_list();
    grid->empty_columns = new_list();
    grid->galaxies = new_list();
    grid->expanded_galaxies = new_list();
    for (int row = 0; row < grid->rows; row++) {
        add_int(grid->empty_rows, row);
    }
    for (int column = 0; column < grid->columns; column++) {
        add_int(grid->empty_columns, column);
    }
    for (int row = 0; row < grid->rows; row++) {
        for (int column = 0; column < grid->columns; column++) {
            if (grid->data[row * grid->columns + column] == '#') {
                remove_int(grid->empty_rows, row);
                remove_int(grid->empty_columns, column);
                add_point(grid->galaxies, row, column);
            }
        }
    }
}

static void get_expanded_galaxies(struct grid *grid, size_t scale) {
    for (struct list_item *galaxy = grid->galaxies->head; galaxy != NULL; galaxy = galaxy->next) {
        size_t row = galaxy->point_value->row;
        size_t column = galaxy->point_value->column;
        size_t extra_rows = 0;
        size_t extra_columns = 0;
        for (struct list_item *item = grid->empty_rows->head; item != NULL; item = item->next) {
            if (*item->int_value < row) {
                extra_rows += scale;
            }
        }
        for (struct list_item *item = grid->empty_columns->head; item != NULL; item = item->next) {
            if (*item->int_value < column) {
                extra_columns += scale;
            }
        }
        add_point(grid->expanded_galaxies, row + extra_rows, column + extra_columns);
    }
}

static size_t get_manhattan_distance(struct point *a, struct point *b) {
    size_t row_distance = a->row > b->row ? a->row - b->row : b->row - a->row;
    size_t column_distance = a->column > b->column ? a->column - b->column : b->column - a->column;
    return row_distance + column_distance;
}

static size_t get_all_manhattan_distances(struct grid *grid) {
    size_t sum = 0;
    for (struct list_item *galaxy_a = grid->expanded_galaxies->head; galaxy_a != NULL; galaxy_a = galaxy_a->next) {
        for (struct list_item *galaxy_b = galaxy_a->next; galaxy_b != NULL; galaxy_b = galaxy_b->next) {
            sum += get_manhattan_distance(galaxy_a->point_value, galaxy_b->point_value);
        }
    }
    return sum;
}

void day11_tests() {
    struct list *lines = load_file("data/day11/test.txt");
    for (struct list_item *line = lines->head; line != NULL; line = line->next) {
        printf("%s\n", line->str_value);
    }
    struct grid *grid = new_grid(lines->items, strlen(lines->head->str_value));
    populate_grid(grid, lines);
    find_empty_rows_and_columns(grid);
    for (struct list_item *item = grid->empty_rows->head; item != NULL; item = item->next) {
        printf("Empty row: %lld\n", *item->int_value);
    }
    for (struct list_item *item = grid->empty_columns->head; item != NULL; item = item->next) {
        printf("Empty column: %lld\n", *item->int_value);
    }
    for (struct list_item *item = grid->galaxies->head; item != NULL; item = item->next) {
        printf("Galaxy: (%lu, %lu)\n", item->point_value->row, item->point_value->column);
    }

    get_expanded_galaxies(grid, 1);
    for (struct list_item *item = grid->expanded_galaxies->head; item != NULL; item = item->next) {
        printf("Expanded galaxy (part 1): (%lu, %lu)\n", item->point_value->row, item->point_value->column);
    }
    size_t sum1 = get_all_manhattan_distances(grid);
    printf("Sum of Manhattan distances between galaxy pairs (part 1): %lu\n", sum1);

    destroy_list(grid->expanded_galaxies);
    grid->expanded_galaxies = new_list();

    get_expanded_galaxies(grid, 999999);
    for (struct list_item *item = grid->expanded_galaxies->head; item != NULL; item = item->next) {
        printf("Expanded galaxy (part 2): (%lu, %lu)\n", item->point_value->row, item->point_value->column);
    }
    size_t sum2 = get_all_manhattan_distances(grid);
    printf("Sum of Manhattan distances between galaxy pairs (part 2): %lu\n", sum2);
    destroy_list(grid->expanded_galaxies);

    destroy_list(grid->galaxies);
    destroy_list(grid->empty_columns);
    destroy_list(grid->empty_rows);
    free(grid);
    destroy_list(lines);
}

unsigned long long day11_part1() {
    struct list *lines = load_file("data/day11/input.txt");
    struct grid *grid = new_grid(lines->items, strlen(lines->head->str_value));
    populate_grid(grid, lines);
    find_empty_rows_and_columns(grid);
    get_expanded_galaxies(grid, 1);
    return get_all_manhattan_distances(grid);
}

unsigned long long day11_part2() {
    struct list *lines = load_file("data/day11/input.txt");
    struct grid *grid = new_grid(lines->items, strlen(lines->head->str_value));
    populate_grid(grid, lines);
    find_empty_rows_and_columns(grid);
    get_expanded_galaxies(grid, 999999);
    return get_all_manhattan_distances(grid);
}
