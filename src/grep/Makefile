CC = gcc
FlAG = -Wall -Werror -Wextra -std=c11

all : s21_grep clean

s21_grep : s21_grep.o
	$(CC) $(FlAG) s21_grep.o -o s21_grep

s21_grep.o : s21_grep.c
	$(CC) $(FlAG) -c s21_grep.c

tests : s21_grep
	tr -d '\r' < tests.sh > tests_fixed.sh
	chmod u+x tests_fixed.sh
	bash tests_fixed.sh

clean:
	rm -rf s21_grep
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
	rm -rf *.log
	rm -rf *.dSYM