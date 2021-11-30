#!/bin/bash

if [ "$1" != "" ]; then
    day=$1
    dayString=$1
     
    if [[ $dayString == [0-9] ]]; then
        dayString="0${day}"
    fi

    mkdir "day${dayString}" 2> /dev/null
    cd "day${dayString}"

    for ((part=1; part <= 2; part++)); do
    cat > "part${part}.c" <<EOF
/* Day ${day}, part ${part} = ? */

#include <stdio.h>

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        int answer = 0;

        fclose(inputFile);

        printf("%d", answer);
    }

    return 0;
}
EOF
    done

    touch input.txt
fi