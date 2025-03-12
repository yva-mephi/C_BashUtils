CC = gcc
FlAG = -Wall -Werror -Wextra -std=c11

all : s21_cat clean

s21_cat : s21_cat.o
	$(CC) $(FlAG) s21_cat.o -o s21_cat

s21_cat.o : s21_cat.c
	$(CC) $(FlAG) -c s21_cat.c

tests : s21_cat
	tr -d '\r' < tests.sh > tests_fixed.sh
	chmod u+x tests_fixed.sh
	bash tests_fixed.sh

clean:
	rm -rf s21_cat
	rm -rf *.o
	rm -rf *.a
	rm -rf *.so
	rm -rf *.log
	rm -rf *.dSYM