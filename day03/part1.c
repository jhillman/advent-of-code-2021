/* Day 3, part 1 = 749376 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bits.h"

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char bits[16];
        int bitsLength = 0;
        struct BitCount *bitCounts = NULL;

        while (!feof(inputFile) && fscanf(inputFile, "%s", bits)) {
            if (!bitCounts) {
                bitsLength = strlen(bits);
                bitCounts = (struct BitCount *)calloc(bitsLength, sizeof(struct BitCount));
            }

            char *bit = bits;

            while (*bit) {
                bitCounts[bit - bits].zeros += *bit == '0' ? 1 : 0;
                bitCounts[bit - bits].ones += *bit == '1' ? 1 : 0;

                ++bit;
            }
        }

        fclose(inputFile);

        int gamma = 0;
        int epsilon = 0;
        int bit = 1;

        for (int i = bitsLength - 1; i >= 0; i--) {
            gamma += bitCounts[i].ones > bitCounts[i].zeros ? bit : 0;
            epsilon += bitCounts[i].zeros > bitCounts[i].ones ? bit : 0;

            bit *= 2;
        }

        free(bitCounts);

        printf("%d", gamma * epsilon);
    }

    return 0;
}
