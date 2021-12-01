/* Day 1, part 1 = 1688 */

#include <stdio.h>

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        int previousMeasurement = 0;
        int measurement;
        int answer = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%d", &measurement)) {
            if (previousMeasurement && measurement > previousMeasurement) {
                ++answer;
            }

            previousMeasurement = measurement;
        }

        fclose(inputFile);

        printf("%d", answer);
    }

    return 0;
}
