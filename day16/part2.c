/* Day 16, part 2 = ? */

#include "bits.h"

int main() {
    char *transmission = getTransmission();

    if (transmission) {
        char *packet = transmission;
        int length = 0;

        int answer = readPacket(packet, &length);

        free(transmission);

        printf("%d", answer);
    }

    return 0;
}