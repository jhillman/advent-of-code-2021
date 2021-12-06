/* Day 6, part 2 = 1675781200288 */

#include <stdio.h>

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        long timers[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
        long timer;
        char ch;
        long answer = 0;

        while (fscanf(inputFile, "%ld%c", &timer, &ch) >= 1) {
            ++timers[timer];
        }

        fclose(inputFile);
        
        for (int i = 0; i < 256; i++) {
            long newFishCount = *timers;

            for (timer = 0; timer < 8; timer++) {
                timers[timer] = timers[timer + 1];
            }

            timers[6] += newFishCount;
            timers[8] = newFishCount;
        }

        for (int i = 0; i < 9; i++) {
            answer += timers[i];
        }

        printf("%ld", answer);
    }

    return 0;
}
