/* Day 17, part 1 = 30628 */

#include "shoot.h"

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        int targetMinX;
        int targetMaxX;
        int targetMinY;
        int targetMaxY;

        fscanf(inputFile, "target area: x=%d..%d, y=%d..%d", &targetMinX, &targetMaxX, &targetMinY, &targetMaxY);

        fclose(inputFile);

        int answer = shoot(targetMinX, targetMaxX, targetMinY, targetMaxY).maxHeight;

        printf("%d", answer);
    }

    return 0;
}
