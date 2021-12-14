/* Day 14, part 2 = 2911561572630 */

#include "polymer.h"

int main() {
    struct PolymerData *data = getPolymerData();

    if (data) {
        long answer = performSteps(data, 40);

        freePolymerData(data);

        printf("%ld", answer);
    }

    return 0;
}
