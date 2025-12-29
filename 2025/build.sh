#!/bin/bash

OUT="./out"
CC="/usr/bin/clang++"
CC_LOG="$OUT/build.log"

CCFLAGS="-Wall -Werror -Wextra -Wunused -Wcast-align -pedantic -pedantic-errors -Wfloat-equal -Wpointer-arith"
O_FLAGS="-O3"
STD_FLAGS="-std=c++20"

FLAGS="$CCFLAGS $O_FLAGS $STD_FLAGS"

rm -fr $OUT
mkdir $OUT

for num in {01..12};
do
    SRC="./task$num"
    for subnum in {1..2};
    do
        echo "$CC $FLAGS "$SRC/task${num}_${subnum}.cpp" -o "$OUT/task${num}_${subnum}.out" &>> $CC_LOG"
        $CC $FLAGS "$SRC/task${num}_${subnum}.cpp" -o "$OUT/task${num}_${subnum}.out" &>> $CC_LOG
        echo "========================================================================================="
    done
done
