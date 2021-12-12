/* Day 12, part 2 = 148962 */

#include "caves.h"

int main() {
    struct CaveData *data = getCaveData();

    if (data) {
        int answer = pathCount(data, data->start, NULL, true);

        freeCaveData(data);

        printf("%d", answer);
    }

    return 0;
}
