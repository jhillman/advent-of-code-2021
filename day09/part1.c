/* Day 9, part 1 = 498 */

#include "map.h"

int main() {
    struct MapData *data = getMapData();

    if (data) {
        int answer = 0;

        for (int y = 0; y < data->height; y++) {
            for (int x = 0; x < data->width; x++) {
                if (isLowPoint(data, y, x)) {
                    answer += data->map[y][x] + 1;
                }
            }
        }

        freeMapData(data);

        printf("%d", answer);
    }

    return 0;
}
