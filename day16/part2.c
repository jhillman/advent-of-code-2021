/* Day 16, part 2 = 246761930504 */

#include "bits.h"

int main() {
    char *transmission = getTransmission();

    if (transmission) {
        char *packet = transmission;
        struct ExpressionPart *expression = (struct ExpressionPart *)calloc(1, sizeof(struct ExpressionPart));

        readPacket(packet, NULL, expression);

        free(transmission);

        long answer = evaluate(expression);

        freeExpression(expression);

        printf("%ld", answer);
    }

    return 0;
}