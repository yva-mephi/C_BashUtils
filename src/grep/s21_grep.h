#ifndef S21_GREP_H
#define S21_GREP_H
#define _GNU_SOURCE

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const struct option long_options[] = {{NULL, 0, NULL, 0}};

void find_options(int argc, char **argv, int *flags, char *patterns_e,
                  int *flag_e, int *flag_f, int *size_f, char **file_name_for_f,
                  int *check);
void f_arg(int *flag_f, char **file_name_for_f, char *patterns_f);
void fsize(char *file_name, int *check, int *size_f);
void sum_f_e(char *patterns, char *patterns_e, char *patterns_f, int *flag_e,
             int *flag_f);
void file_open(int argc, char **argv, FILE *txt, char *str, char *file_name,
               int *flag_e, char *patterns, int *number_of_files,
               int *number_of_patterns, int *flags);
void read_str(FILE *txt, char *str, int *number_of_files, char *file_name,
              regex_t *regex, int *flags, int *cnt, int *str_cnt);
void output(char *str, int *number_of_files, char *file_name, regex_t *regex,
            int *flags, int *cnt, int *str_cnt);
void func_o(char *str, regex_t *regex);

#endif