/* Day 1, part 2 = 1728 */

#include <stdio.h>

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        int sums[3] = {0, 0, 0};
        int addendCounts[3] = {0, 0, 0};
        int measurementCount = 1;
        int measurement;
        int previousSum = 0;
        int answer = 0;

        while (!feof(inputFile) && fscanf(inputFile, "%d", &measurement)) {
            for (int i = 0; i < measurementCount; i++) {
                sums[i] += measurement;

                if (++addendCounts[i] == 3) {
                    if (previousSum && sums[i] > previousSum) {
                        ++answer;
                    }

                    previousSum = sums[i];

                    sums[i] = 0;
                    addendCounts[i] = 0;
                }
            }

            if (measurementCount < 3) {
                ++measurementCount;
            }
        }

        fclose(inputFile);

        printf("%d", answer);
    }

    return 0;
}
