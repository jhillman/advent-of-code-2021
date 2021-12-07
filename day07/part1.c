/* Day 7, part 1 = 333755 */

#include "crabs.h"

int main() {
    struct CrabData *data = getCrabData();

    if (data) {
        int answer = 1000000000;

        for (int position = data->minPosition; position <= data->maxPosition; position++) {
            int fuel = 0;

            for (int i = 0; i < data->crabCount; i++) {
                fuel += labs(data->positions[i] - position);
            }

            if (fuel <= answer) {
                answer = fuel;
            }
        }

        freeCrabData(data);

        printf("%d", answer);
    }

    return 0;
}
