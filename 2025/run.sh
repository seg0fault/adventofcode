#!/bin/bash

OUT="./out"
TESTS="./tests"

for task_num in {01..12};
do
    for task_subnum in {1..2};
    do
        EXE_NAME="task${task_num}_${task_subnum}"
        EXE="${OUT}/$EXE_NAME.out"

        if [ ! -f $EXE ]; then
          echo "$EXE_NAME NOT FOUND"
          echo ""
          continue
        fi

        TEST_NAME="test${task_num}_2"
        TEST="${TESTS}/$TEST_NAME.txt"

        echo "$EXE $TEST"
        $EXE $TEST

        echo ""
    done
done
