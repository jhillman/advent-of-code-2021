/* Day 14, part 1 = 2657 */

#include "polymer.h"

int main() {
    struct PolymerData *data = getPolymerData();

    if (data) {
        long answer = performSteps(data, 10);

        freePolymerData(data);

        printf("%ld", answer);
    }

    return 0;
}
