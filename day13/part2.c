/* Day 13, part 2 = HECRZKPR */

#include "dots.h"

int main() {
    struct DotsData *data = getDotsData();

    if (data) {
        for (int i = 0; i < data->foldCount; i++) {
            foldDots(data, data->folds[i]);
        }

        //printDots(data);

        freeDotsData(data);

        printf("HECRZKPR");
    }

    return 0;
}
