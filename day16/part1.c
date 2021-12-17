/* Day 16, part 1 = 1038 */

#include "bits.h"

int main() {
    char *transmission = getTransmission();

    if (transmission) {
        char *packet = transmission;

        int answer = readPacket(packet, NULL, NULL);

        free(transmission);

        printf("%d", answer);
    }

    return 0;
}
