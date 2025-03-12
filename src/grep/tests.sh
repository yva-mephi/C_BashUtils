#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0

if [ ! -x "./s21_grep" ]; then
    echo "Error: ./s21_grep command not found"
    exit 1
fi

if [ ! -x "$(which grep)" ]; then
    echo "Error: grep command not found"
    exit 1
fi

tests=(
    "hello test_files/test1.txt"
    "hello test_files/test1.txt test_files/test2.txt"
    "-e c test_files/test1.txt"
    "-i school test_files/test3.txt test_files/test4.txt"
    "-e hello -e double -e delo -e frek -v test_files/test3.txt -v"
    "-e hello -f test_files/test4.txt test_files/test3.txt -c"
    "-e hello -e for test_files/test5.txt test_files/test2.txt test_files/test1.txt -l"
    "-e hello test_files/test1.txt test_files/test2.txt -n"
    "-e hello test_files/test1.txt test_files/test2.txt -h"
    "-e hello -f test_files/test3.txt test_files/test2.txt test_files/test3.txt test_files/test.txt test_files/test5.txt -s"
    "-e hello -iv test_files/test3.txt"
    "-f test_files/test3.tt  -f test_files/test4.txt test_files/test1.txt -s"
    "-e hello -in test_files/test1.txt -s"
    "-e hello -iv test_files/test1.txt -s"
    "-e hello -iv test_files/test1.xt -s"
    "-e hello -iv -f test_files/test4.xt test_files/test1.xt -s"
)

run_test() {
    param="$@"
    ./s21_grep $param > s21_grep.log
    grep $param > grep.log
    DIFF=$(diff -s s21_grep.log grep.log)
    let "COUNTER++"
    if [ "$DIFF" == "Files s21_grep.log and grep.log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
    else
        let "FAIL++"
        echo "$COUNTER - Fail $param"
    fi
    rm -f s21_grep.log grep.log
}

for i in "${tests[@]}"
do
    run_test $i
done

printf "\n"
echo "FAILED: $FAIL"
echo "SUCCESSFUL: $SUCCESS"
echo "ALL: $COUNTER"
printf "\n"
