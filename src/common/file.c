//
// Created by keith on 11/12/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"
#include "list.h"

#define BUFFER_SIZE 1024

struct list *load_file(char *filename) {
    char buffer[BUFFER_SIZE];
    FILE *handle = fopen(filename, "r");
    if (handle == NULL) {
        perror("get_file::fopen");
        exit(1);
    }
    struct list *lines = new_list();
    if (lines == NULL) {
        perror("get_file::malloc (lines)");
        exit(1);
    }
    char *result = fgets(buffer, BUFFER_SIZE, handle);
    while (result != NULL) {
        size_t size = strlen(result);
        while (size > 0 && (result[size - 1] == '\n' || result[size - 1] == '\r')) {
            result[size - 1] = '\0';
            size--;
        }
        add_str(lines, buffer);
        result = fgets(buffer, BUFFER_SIZE, handle);
    }
    fclose(handle);
    return lines;
}
