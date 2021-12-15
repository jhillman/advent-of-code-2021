/* Day 15, part 2 = 2821 */

#include "chitons.h"

int main() {
    struct ChitonsData *data = getChitonsData(5);

    if (data) {
        int originalWidth = data->width / 5;
        int originalHeight = data->height / 5;

        for (int y = 0; y < data->height; y++) {
            for (int x = 0; x < data->width; x++) {
                if (y >= originalHeight || x >= originalWidth) {
                    int level = y >= originalHeight ? 
                        data->riskLevels[y - originalHeight][x] : 
                        data->riskLevels[y][x - originalWidth];

                    if (++level > 9) {
                        level = 1;
                    }

                    data->riskLevels[y][x] = level;
                }
            }
        }

        int answer = lowestRisk(data);

        freeChitonsData(data);

        printf("%d", answer);
    }

    return 0;
}
