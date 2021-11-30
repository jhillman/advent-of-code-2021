#!/bin/bash

for directory in $( find ./ -type d -name "day*" | sort )
do
    pushd . > /dev/null
    cd $directory

	day=`grep -m 1 -o '[0-9]\+' part1.c | head -1`

    echo "Building day $day..."

    gcc part1.c -O3 -o part1.o
    gcc part2.c -O3 -o part2.o

    popd > /dev/null
done
