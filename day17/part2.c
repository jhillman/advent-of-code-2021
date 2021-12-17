/* Day 17, part 2 = 4433 */

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
            for (int y = -300; y < 300; y++) {
                if (shoot(x, y, targetMinX, targetMaxX, targetMinY, targetMaxY).hitTarget) {
                    ++answer;
                }
            }
        }

        printf("%d", answer);
    }

    return 0;
}
