#!/bin/bash

SUCCESS=0
FAIL=0
COUNTER=0

if [ ! -x "./s21_cat" ]; then
    echo "Error: ./s21_cat command not found"
    exit 1
fi

if [ ! -x "$(which cat)" ]; then
    echo "Error: cat command not found"
    exit 1
fi

tests=(
    "-b test_files/test1.txt"
    "-e test_files/test1.txt test_files/test2.txt"
    "-n test_files/test1.txt"
    "-s test_files/test3.txt test_files/test4.txt"
    "-t test_files/test3.txt"
    "-v test_files/test1.txt test_files/test2.txt"
    "test_files/test3.txt test_files/test2.txt test_files/test3.txt test_files/test5.txt"
    "-bn test_files/test3.txt"
    "-bs test_files/test1.txt "
    "-vt test_files/test1.txt "
    "-bnst test_files/test1.txt"
    "-benst test_files/test1.xt"
    "-t test_files/1.txt"
    "-v test_files/1.txt"
    "-e test_files/1.txt"
    "-t test_files/1.txt"
)

run_test() {
    param="$@"
    ./s21_cat $param > s21_cat.log
    cat $param > cat.log
    DIFF=$(diff -s s21_cat.log cat.log)
    let "COUNTER++"
    if [ "$DIFF" == "Files s21_cat.log and cat.log are identical" ]
    then
        let "SUCCESS++"
        echo "$COUNTER - Success $param"
    else
        let "FAIL++"
        echo "$COUNTER - Fail $param"
    fi
    rm -f s21_cat.log cat.log
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
