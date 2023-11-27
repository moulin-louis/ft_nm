#!/bin/bash

##define function to check if the test is ok or not

function run_diff_null {
  diff diff1 diff2 > /dev/null
  return $?
}

function run_diff() {
    diff --color=auto -y diff1 diff2
}

function check_test {
    run_diff_null
    ret=$?
    if [ $ret -eq 0 ]; then
        echo "TEST $1: OK"
    else
        echo "TEST $1: KO"
        run_diff
        exit 1
    fi
}

./cmake-build-debug/ft_nm ./sample/test.o > diff1
nm ./sample/test.o > diff2
check_test 1
./cmake-build-debug/ft_nm ./sample/test > diff1
nm ./sample/test > diff2
check_test 2
./cmake-build-debug/ft_nm ./sample/test -r > diff1
nm ./sample/test -r > diff2
check_test 3
./cmake-build-debug/ft_nm ./sample/test -p > diff1
nm ./sample/test -p > diff2
check_test 4
./cmake-build-debug/ft_nm ./sample/test -a > diff1
nm ./sample/test -a > diff2
check_test 5
./cmake-build-debug/ft_nm ./sample/test -g > diff1
nm ./sample/test -g > diff2
check_test 6
./cmake-build-debug/ft_nm ./sample/test -u > diff1
nm ./sample/test -u > diff2
check_test 7
