/* Day 16, part 1 = 1038 */

#include "bits.h"

int main() {
    char *transmission = getTransmission();

    if (transmission) {
        char *packet = transmission;
        struct ExpressionNode *expression = (struct ExpressionNode *)calloc(1, sizeof(struct ExpressionNode));

        readPacket(packet, NULL, expression);

        free(transmission);

        long answer = versionSum(expression);

        freeExpression(expression);

        printf("%ld", answer);
    }

    return 0;
}
