/* Day 11, part 2 = 371 */

#include "octopuses.h"

int main() {
    struct OctopusData *data = getOctopusData();

    if (data) {
        int answer = 0;
        int stepNumber = 0;

        while (!answer) {
            ++stepNumber;

            if (step(data) == GRID_SIZE * GRID_SIZE) {
                answer = stepNumber;
            }
        }

        freeOctopusData(data);

        printf("%d", answer);
    }

    return 0;
}
