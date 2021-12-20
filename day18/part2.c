/* Day 18, part 2 = 4819 */

#include "snailfish.h"

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        int numberCount = 0;
        char numberString[64];
        struct SnailfishNumber **numbers;
        long answer = 0;

        while (fgets(numberString, sizeof(numberString), inputFile)) {
            ++numberCount;
        }

        fseek(inputFile, 0, SEEK_SET);

        numbers = (struct SnailfishNumber **)calloc(numberCount, sizeof(struct SnailfishNumber *));

        for (int i = 0; i < numberCount; i++) {
            fgets(numberString, sizeof(numberString), inputFile);

            numbers[i] = readNumber(NULL, numberString, NULL);
        }

        fclose(inputFile);

        for (int i = 0; i < numberCount; i++) {
            for (int j = 0; j < numberCount; j++) {
                if (i != j) {
                    struct SnailfishNumber *sum = add(copy(numbers[i], NULL), copy(numbers[j], NULL));

                    long sumMagnitude = magnitude(sum);

                    if (sumMagnitude > answer) {
                        answer = sumMagnitude;
                    }

                    freeNumber(sum);
                }
            }
        }

        for (int i = 0; i < numberCount; i++) {
            freeNumber(numbers[i]);
        }

        free(numbers);

        printf("%ld", answer);
    }

    return 0;
}
