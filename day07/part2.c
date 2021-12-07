/* Day 7, part 2 = 94017638 */

#include "crabs.h"

int main() {
    struct CrabData *data = getCrabData();

    if (data) {
        long answer = 1000000000;

        for (long position = data->minPosition; position <= data->maxPosition; position++) {
            long fuel = 0;

            for (long i = 0; i < data->crabCount; i++) {
                long distance = labs(data->positions[i] - position);

                fuel += distance > 1 ? (long)(((float)distance / 2) * (distance + 1)) : distance;
            }

            answer = min(answer, fuel);
        }

        freeCrabData(data);

        printf("%ld", answer);
    }

    return 0;
}
