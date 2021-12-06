/* Day 6, part 1 = 372300 */

#include <stdio.h>
#include <stdlib.h>

struct LanternFish {
    int timer;
    struct LanternFish *next;
};

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        struct LanternFish *school = (struct LanternFish *)calloc(1, sizeof(struct LanternFish));
        struct LanternFish *currentFish = school;
        struct LanternFish *lastFish;
        char ch;
        int answer = 0;

        while (fscanf(inputFile, "%d%c", &currentFish->timer, &ch) >= 1) {
            lastFish = currentFish;

            if (ch == ',') {
                currentFish->next = (struct LanternFish *)calloc(1, sizeof(struct LanternFish));
                currentFish = currentFish->next;
            }

            ch = '\0';
        }

        fclose(inputFile);
        
        for (int i = 0; i < 80; i++) {
            currentFish = school;
            int newFishCount = 0;

            while (currentFish) {
                if (currentFish->timer == 0) {
                    currentFish->timer = 6;
                    ++newFishCount;
                } else {
                    --currentFish->timer;
                }

                currentFish = currentFish->next;
            }

            while (newFishCount > 0) {
                lastFish->next = (struct LanternFish *)calloc(1, sizeof(struct LanternFish));
                lastFish = lastFish->next;
                lastFish->timer = 8;

                --newFishCount;
            }
        }

        while (school) {
            currentFish = school->next;

            free(school);

            ++answer;
            school = currentFish;
        }

        printf("%d", answer);
    }

    return 0;
}
