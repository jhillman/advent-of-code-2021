/* Day 12, part 1 = 4659 */

#include "caves.h"

int main() {
    struct CaveData *data = getCaveData();

    if (data) {
        char *visitedSmallCaves = (char *) calloc(100, sizeof(char));

        int answer = pathCount(data, data->start, visitedSmallCaves, false);

        free(visitedSmallCaves);
        freeCaveData(data);

        printf("%d", answer);
    }

    return 0;
}
