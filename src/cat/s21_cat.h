#ifndef S21_CAT_H
#define S21_CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"", no_argument, NULL, 'e'},
    {"show-ends", no_argument, NULL, 'E'},
    {"show-tabs", no_argument, NULL, 'T'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'},
    {"show-nonprinting", no_argument, NULL, 'v'},
    {"", no_argument, NULL, 't'},
    {NULL, 0, NULL, 0}};

void find_options(int *flags, int argc, char **argv, int *check);
void print(int *flags, char *argv, int *check, int *str);
void check_s(char *prev_num, char *num, int *flag_s);
void check_s(char *prev_num, char *num, int *flag_s);
void print_b(int *flags, char *num, int *str);
void print_n(int *flags, char *num, int *str);

#endif
