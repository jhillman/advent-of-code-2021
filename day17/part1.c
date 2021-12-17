/* Day 17, part 1 = 30628 */

#include "shoot.h"

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        int targetMinX;
        int targetMaxX;
        int targetMinY;
        int targetMaxY;
        int answer = 0;

        fscanf(inputFile, "target area: x=%d..%d, y=%d..%d", &targetMinX, &targetMaxX, &targetMinY, &targetMaxY);

        fclose(inputFile);

        for (int x = 0; x < 100; x++) {
            for (int y = 0; y < 300; y++) {
                struct ShotResult result = shoot(x, y, targetMinX, targetMaxX, targetMinY, targetMaxY);

                if (result.hitTarget && result.maxHeight > answer) {
                    answer = result.maxHeight;
                }
            }
        }

        printf("%d", answer);
    }

    return 0;
}
