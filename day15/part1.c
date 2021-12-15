/* Day 15, part 1 = 487 */

#include "chitons.h"

int main() {
    struct ChitonsData *data = getChitonsData(1);

    if (data) {
        int answer = lowestRisk(data);

        freeChitonsData(data);

        printf("%d", answer);
    }

    return 0;
}
