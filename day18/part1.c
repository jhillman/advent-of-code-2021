/* Day 18, part 1 = 3816 */

#include "snailfish.h"

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char numberString[64];
        struct SnailfishNumber *number = NULL;

        while (fgets(numberString, sizeof(numberString), inputFile)) {
            if (number) {
                number = add(number, readNumber(NULL, numberString, NULL));
            } else {
                number = readNumber(NULL, numberString, NULL);
            }
        }

        fclose(inputFile);

        long answer = magnitude(number);

        freeNumber(number);

        printf("%ld", answer);
    }

    return 0;
}
