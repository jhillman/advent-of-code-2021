/* Day 4, part 1 = 28082 */

#include "bingo.h"

int main() {
    struct BingoData *data = getBingoData();

    if (data) {
        int answer = 0;

        for (int i = 0; !answer && i < data->numberCount; i++) {
            int number = data->numbers[i];

            for (int j = 0; !answer && j < data->boardCount; j++) {
                playNumber(&data->boards[j], number);

                if (!data->boards[j].winner && boardWins(&data->boards[j])) {
                    answer = number * data->boards[j].unmarkedSum;
                }
            }
        }

        freeBingoData(data);

        printf("%d", answer);
    }

    return 0;
}
