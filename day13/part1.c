/* Day 13, part 1 = 759 */

#include "dots.h"

int main() {
    struct DotsData *data = getDotsData();

    if (data) {
        int answer = foldDots(data, *data->folds);

        freeDotsData(data);

        printf("%d", answer);
    }

    return 0;
}
