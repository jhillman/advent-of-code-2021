#!/bin/bash

zig=0

for arg in "$@"
do
    case $arg in
        -z|--zig)
        zig=1
        shift
        ;;
    esac
done

for directory in $( find ./ -type d -name "day*" | sort )
do
    pushd . > /dev/null
    cd $directory

    day=`grep -m 1 -o '[0-9]\+' part1.c | head -1`

    if [ $zig == 1 ]
    then
        echo "Building day $day with zig..."

        zig cc -O3 part1.c -o part1.o
        zig cc -O3 part2.c -o part2.o
    else
        echo "Building day $day with gcc..."

        gcc part1.c -O3 -o part1.o
        gcc part2.c -O3 -o part2.o
    fi

    popd > /dev/null
done
