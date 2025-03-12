#include "s21_grep.h"

int main(int argc, char **argv) {
  if (argc > 2) {
    int check = 1;
    int flags[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int size_e = argc;
    for (int i = 1; i < argc; i++) {
      size_e += strlen(argv[i]);
    }
    char *patterns_e = calloc(size_e, sizeof(char));
    char **file_name_for_f = calloc(argc, sizeof(char *));
    int size_f = 0, flag_e = 0, flag_f = 0;
    char *patterns = NULL, *patterns_f = NULL;
    find_options(argc, argv, flags, patterns_e, &flag_e, &flag_f, &size_f,
                 file_name_for_f, &check);
    if (check == 0) {
      fprintf(stderr, "grep: %s: No such file or directory",
              file_name_for_f[flag_f - 1]);
      exit(1);
    } else {
      patterns = calloc((size_f + size_e), sizeof(char));
      if (flag_f > 0) {
        patterns_f = calloc(size_f, sizeof(char));
        f_arg(&flag_f, file_name_for_f, patterns_f);
      }
      if (flag_f > 0 || flag_e > 0) {
        sum_f_e(patterns, patterns_e, patterns_f, &flag_e, &flag_f);
      }
      FILE *txt = NULL;
      char *str = NULL, *file_name = NULL;
      int number_of_files = 0, number_of_patterns = 0;
      file_open(argc, argv, txt, str, file_name, &flag_e, patterns,
                &number_of_files, &number_of_patterns, flags);
    }
    free(patterns_e);
    free(patterns_f);
    free(patterns);
    free(file_name_for_f);
  } else {
    fprintf(stderr, "ERROR\n");
    exit(2);
  }
  return 0;
}

void find_options(int argc, char **argv, int *flags, char *patterns_e,
                  int *flag_e, int *flag_f, int *size_f, char **file_name_for_f,
                  int *check) {
  int opt = 0, option_index = 1;
  const char *short_options = "e:ivclnhsf:o";
  while ((opt = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (opt) {
      case 'e':
        flags[0] = 1;
        strcat(patterns_e, optarg);
        strcat(patterns_e, "|");
        (*flag_e)++;
        break;
      case 'i':
        flags[1] = 1;
        break;
      case 'v':
        flags[2] = 1;
        break;
      case 'c':
        flags[3] = 1;
        break;
      case 'l':
        flags[4] = 1;
        break;
      case 'n':
        flags[5] = 1;
        break;
      case 'h':
        flags[6] = 1;
        break;
      case 's':
        flags[7] = 1;
        break;
      case 'f':
        flags[8] = 1;
        fsize(optarg, check, size_f);
        file_name_for_f[*flag_f] = optarg;
        (*flag_f)++;
        break;
      case 'o':
        flags[9] = 1;
        break;
    }
    if (*check == 0) {
      break;
    }
  }
}

void fsize(char *file_name, int *check, int *size_f) {
  int sz = 0;
  FILE *fp = fopen(file_name, "r");
  if (fp != NULL) {
    fseek(fp, 0L, SEEK_END);
    sz = ftell(fp);
    sz = sz + 2;
    *size_f = *size_f + sz;
    fclose(fp);
  } else {
    *check = 0;
  }
}

void f_arg(int *flag_f, char **file_name_for_f, char *patterns_f) {
  char *str = NULL;
  for (int i = 0; i < *flag_f; i++) {
    FILE *f = fopen(file_name_for_f[i], "r");
    size_t buff = 0;
    if (i > 0) {
      strcat(patterns_f, "|");
    }
    while (getline(&str, &buff, f) != -1) {
      if (strcmp(str, "\n\0") != 0) {
        strcat(patterns_f, str);
      }
    }
    int s = strlen(patterns_f);
    for (int j = 0; j < s; j++) {
      if (patterns_f[j] == '\n') {
        patterns_f[j] = '|';
      }
    }
    free(str);
    fclose(f);
    str = NULL;
  }
  patterns_f[strlen(patterns_f)] = '\0';
}

void sum_f_e(char *patterns, char *patterns_e, char *patterns_f, int *flag_e,
             int *flag_f) {
  if (*flag_f > 0 && *flag_e > 0) {
    strcat(patterns, patterns_e);
    strcat(patterns, patterns_f);
  } else if (*flag_f > 0 && *flag_e == 0) {
    strcat(patterns, patterns_f);
  } else if (*flag_f == 0 && *flag_e > 0) {
    strcat(patterns, patterns_e);
  }
  if (patterns[strlen(patterns) - 1] == '|') {
    patterns[strlen(patterns) - 1] = '\0';
  } else if (patterns[strlen(patterns)] == '|') {
    patterns[strlen(patterns) - 1] = '\0';
  }
  *flag_e = *flag_e + *flag_f;
}

void file_open(int argc, char **argv, FILE *txt, char *str, char *file_name,
               int *flag_e, char *patterns, int *number_of_files,
               int *number_of_patterns, int *flags) {
  *number_of_patterns = *flag_e;
  if (*flag_e == 0) {
    patterns = argv[optind];
    optind++;
    (*number_of_patterns)++;
  }
  *number_of_files = argc - optind;
  regex_t regex;
  if (flags[1] && !flags[9]) {
    regcomp(&regex, patterns, REG_ICASE | REG_EXTENDED);
  } else if (flags[1] && flags[9]) {
    regcomp(&regex, patterns, REG_ICASE | REG_EXTENDED | REG_NEWLINE);
  } else if (!flags[1] && flags[9]) {
    regcomp(&regex, patterns, REG_EXTENDED | REG_NEWLINE);
  } else {
    regcomp(&regex, patterns, REG_EXTENDED);
  }
  for (int i = optind; i < argc; i++) {
    file_name = argv[i];
    txt = fopen(file_name, "r");
    if (txt != NULL) {
      int cnt = 0;
      int str_cnt = 0;
      read_str(txt, str, number_of_files, file_name, &regex, flags, &cnt,
               &str_cnt);
      if (flags[3]) {
        if (*number_of_files > 1) {
          printf("%s:", file_name);
        }
        printf("%d\n", cnt);
      }
      if (flags[4]) {
        if (cnt > 0) {
          printf("%s\n", file_name);
        }
      }
      fclose(txt);
    } else if (flags[7] == 0) {
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
      exit(3);
    }
  }
  regfree(&regex);
}

void read_str(FILE *txt, char *str, int *number_of_files, char *file_name,
              regex_t *regex, int *flags, int *cnt, int *str_cnt) {
  size_t buff = 0;
  while (getline(&str, &buff, txt) != -1) {
    (*str_cnt)++;
    output(str, number_of_files, file_name, regex, flags, cnt, str_cnt);
  }
  free(str);
  str = NULL;
}

void output(char *str, int *number_of_files, char *file_name, regex_t *regex,
            int *flags, int *cnt, int *str_cnt) {
  int reti = regexec(regex, str, 0, NULL, 0);
  if (!reti && !flags[2]) {
    if (flags[3] == 0 && flags[4] == 0) {
      if (*number_of_files > 1 && flags[6] == 0) {
        printf("%s:", file_name);
      }
      if (flags[5]) {
        printf("%d:", *str_cnt);
      }
      if (flags[9] == 0) {
        printf("%s", str);
      } else if (flags[9]) {
        func_o(str, regex);
      }
      if (str[strlen(str) - 1] != '\n') {
        printf("\n");
      }
    }
    (*cnt)++;
  } else if (reti && flags[2]) {
    if (flags[3] == 0 && flags[4] == 0) {
      if (*number_of_files > 1 && flags[6] == 0) {
        printf("%s:", file_name);
      }
      if (flags[5]) {
        printf("%d:", *str_cnt);
      }
      printf("%s", str);
      if (str[strlen(str) - 1] != '\n') {
        printf("\n");
      }
    }
    (*cnt)++;
  }
}

void func_o(char *str, regex_t *regex) {
  size_t nmatch = 1;
  regmatch_t pmatch[1];
  int offset = 0;
  int regres = 0;
  int j = 0;
  while (regres != 1 && j < 10) {
    regres = regexec(regex, str + offset, nmatch, pmatch, 0);
    if (regres != 1) {
      offset += pmatch[0].rm_eo;
      for (int i = offset - pmatch->rm_eo + pmatch->rm_so; i < offset; i++) {
        printf("%c", str[i]);
      }
      printf("\n");
    }
  }
}