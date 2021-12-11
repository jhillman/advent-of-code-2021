/* Day 11, part 1 = 1620 */

#include "octopuses.h"

int main() {
    struct OctopusData *data = getOctopusData();

    if (data) {
        int answer = 0;

        for (int i = 0; i < 100; i++) {
            answer += step(data);
        }

        freeOctopusData(data);

        printf("%d", answer);
    }

    return 0;
}
