//
// Created by keith on 11/12/2023.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "aoc.h"

void run_tests() {
    day11_tests();
    day12_test_part1();
    day12_test_part2();
}

int main(int argc, char **argv) {
    chdir("/mnt/d/Programming/repos/advent-of-code-2023-c");
    if (argc > 1 && !strcmp(argv[1], "run")) {
        printf("Day 11 Part 1: %lld\n", day11_part1());
        printf("Day 11 Part 2: %lld\n", day11_part2());
        printf("Day 12 Part 1: %lld\n", day12_part1());
        printf("Day 12 Part 2: %lld\n", day12_part2());
    } else {
        run_tests();
    }
    return 0;
}
