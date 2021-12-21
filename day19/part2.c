/* Day 19, part 2 = ? */

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
