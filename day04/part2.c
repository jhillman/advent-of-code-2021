/* Day 4, part 2 = 8224 */

#include "bingo.h"

int main() {
    struct BingoData *data = getBingoData();

    if (data) {
        int answer = 0;
        int losingBoards = data->boardCount;

        for (int i = 0; !answer && i < data->numberCount; i++) {
            int number = data->numbers[i];

            for (int j = 0; !answer && j < data->boardCount; j++) {
                playNumber(&data->boards[j], number);

                if (!data->boards[j].winner && boardWins(&data->boards[j])) {
                    if (--losingBoards == 0) {
                        answer = number * data->boards[j].unmarkedSum;
                    }
                }
            }
        }

        freeBingoData(data);

        printf("%d", answer);
    }

    return 0;
}
