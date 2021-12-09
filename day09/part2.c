/* Day 9, part 2 = 1071000 */

#include "map.h"

#define VISITED -1

int compare(const void *a, const void *b) {
    return *(int *)a - *(int *)b;
}

int basinSize(struct MapData *data, int y, int x) {
    int size = 0;

    if (data->map[y][x] < 9 && data->map[y][x] != VISITED) {
        ++size;
        data->map[y][x] = VISITED;

        if (x > 0) {
            size += basinSize(data, y, x - 1);
        }

        if (x < data->width - 1) {
            size += basinSize(data, y, x + 1);
        }

        if (y > 0) {
            size += basinSize(data, y - 1, x);
        }

        if (y < data->height - 1) {
            size += basinSize(data, y + 1, x);
        }
    }

    return size;
}

int main() {
    struct MapData *data = getMapData();

    if (data) {
        int basinSizes[3] = { 0, 0, 0 };

        for (int y = 0; y < data->height; y++) {
            for (int x = 0; x < data->width; x++) {
                if (isLowPoint(data, y, x)) {
                    int size = basinSize(data, y, x);

                    for (int i = 0; i < 3; i++) {
                        if (size > basinSizes[i]) {
                            basinSizes[i] = size;

                            qsort(basinSizes, 3, sizeof(int), compare);

                            break;
                        }
                    }
                }
            }
        }

        freeMapData(data);

        printf("%d", basinSizes[0] * basinSizes[1] * basinSizes[2]);
    }

    return 0;
}
