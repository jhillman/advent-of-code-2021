/* Day 23, part 1 = 15412 */

#define ROOM_DEPTH 2

#include "amphipods.h"

int main() {
    struct Burrow burrow;

    if (readBurrow("input1.txt", &burrow)) {
        int answer = organizeBurrow(burrow);

        printf("%d", answer);
    }

    return 0;
}
