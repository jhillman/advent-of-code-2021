/* Day 12, part 1 = 4659 */

#include "caves.h"

int main() {
    struct CaveData *data = getCaveData();

    if (data) {
        int answer = pathCount(data, data->start, 0, false);

        freeCaveData(data);

        printf("%d", answer);
    }

    return 0;
}
