//
// Created by keith on 11/12/2023.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "aoc.h"

int main(int argc, char **argv) {
    chdir("/mnt/d/Programming/repos/advent-of-code-2023-c");
    if (argc > 1 && !strcmp(argv[1], "test")) {
        day11_tests();
    } else {
        printf("Day 11 Part 1: %lld\n", day11_part1());
        printf("Day 11 Part 2: %lld\n", day11_part2());
    }
    return 0;
}
