/* Day 23, part 2 = 52358 */

#define ROOM_DEPTH 4

#include "amphipods.h"

int main() {
    struct Burrow burrow;

    if (readBurrow("input2.txt", &burrow)) {
        int answer = organizeBurrow(burrow);

        printf("%d", answer);
    }

    return 0;
}
