#include "s21_cat.h"

int main(int argc, char **argv) {
  int check = 1;
  int flags[] = {0, 0, 0, 0, 0, 0};
  find_options(flags, argc, argv, &check);
  if (check) {
    int str = 1;
    for (int i = 1, j = 0; i < argc; i++) {
      if (argv[i][j] != '-') {
        print(flags, argv[i], &check, &str);
      }
      if (!check) {
        fprintf(stderr, "ERROR\n");
      }
      str = 1;
    }
  } else {
    fprintf(stderr, "ERORR\n");
    exit(2);
  }
  return 0;
};

void find_options(int *flags, int argc, char **argv, int *check) {
  const char *short_options = "bnestETv";
  int rez;
  int option_index;
  opterr = 0;
  while ((rez = getopt_long(argc, argv, short_options, long_options,
                            &option_index)) != -1) {
    switch (rez) {
      case 'b': {
        flags[0] = 1;
        break;
      };
      case 'e': {
        flags[3] = 1;
        flags[5] = 1;
        break;
      };
      case 'E': {
        flags[3] = 1;
        break;
      };
      case 'n': {
        flags[1] = 1;
        break;
      };
      case 's': {
        flags[2] = 1;
        break;
      };
      case 't': {
        flags[4] = 1;
        flags[5] = 1;
        break;
      };
      case 'T': {
        flags[4] = 1;
        break;
      };
      case 'v': {
        flags[5] = 1;
        break;
      };
      case '?':
      default: {
        *check = 0;
        break;
      };
    };
  };
}

void print(int *flags, char *argv, int *check, int *str) {
  char prev_num = '\n';
  char num;
  int flag_s = 0;
  FILE *txt = fopen(argv, "r");
  if (txt != NULL) {
    while ((num = fgetc(txt)) != EOF) {
      if (flags[2]) {
        check_s(&prev_num, &num, &flag_s);
      }
      if (flags[0] && prev_num == '\n' && num != '\n' &&
          (flag_s == 0 || flag_s == 2)) {
        print_b(flags, &num, str);
      } else if (flags[1] && !flags[0] && prev_num == '\n' &&
                 (flag_s == 0 || flag_s == 2)) {
        print_n(flags, &num, str);
      } else if (flags[4] && num == '\t') {
        printf("^I");
      } else if (flags[5] && num < 32 && num != '\n' && num != '\t') {
        printf("^%c", num + 64);
      } else if (flags[5] && num == 127 && num != '\t' && num != '\n') {
        printf("^?");
      } else {
        if (flags[3] && num == '\n' && (flag_s == 0 || flag_s == 2)) {
          printf("$%c", num);
        } else if ((flag_s == 0 || flag_s == 2)) {
          printf("%c", num);
        }
      }
      prev_num = num;
    }
  } else {
    *check = 0;
  }
}

void check_s(char *prev_num, char *num, int *flag_s) {
  if (*prev_num == '\n') {
    if (*num == '\n' && *flag_s == 0) {
      *flag_s = 2;
    } else if (*num == '\n' && *flag_s == 2) {
      *flag_s = 1;
    } else if (*num != '\n') {
      *flag_s = 0;
    }
  }
}

void print_b(int *flags, char *num, int *str) {
  if (flags[4] && *num == '\t') {
    printf("%6.d\t^I", *str);
    *str = *str + 1;
  } else if (flags[5] && *num < 32 && *num != '\n' && *num != '\t') {
    printf("%6.d\t^%c", *str, *num + 64);
    *str = *str + 1;
  } else if (flags[5] && *num == 127 && *num != '\t' && *num != '\n') {
    printf("%6.d\t^?", *str);
    *str = *str + 1;
  } else {
    printf("%6.d\t%c", *str, *num);
    *str = *str + 1;
  }
}

void print_n(int *flags, char *num, int *str) {
  if (flags[4] && *num == '\t') {
    printf("%6.d\t^I", *str);
    *str = *str + 1;
  } else if (flags[3] && *num == '\n') {
    printf("%6.d\t$%c", *str, *num);
    *str = *str + 1;
  } else if (flags[5] && *num < 32 && *num != '\n' && *num != '\t') {
    printf("%6.d\t^%c", *str, *num + 64);
    *str = *str + 1;
  } else if (flags[5] && *num == 127 && *num != '\t' && *num != '\n') {
    printf("%6.d\t^?", *str);
    *str = *str + 1;
  } else {
    printf("%6.d\t%c", *str, *num);
    *str = *str + 1;
  }
}