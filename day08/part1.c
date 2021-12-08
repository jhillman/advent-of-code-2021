/* Day 8, part 1 = 521 */

#include <stdio.h>
#include <string.h>

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        int answer = 0;
        char value[8];
        char ch;

        while (fscanf(inputFile, "%s%c", value, &ch) >= 1) {
            if (strcmp(value, "|") == 0) {
                for (int i = 0; i < 4; i++) {
                    fscanf(inputFile, "%s%c", value, &ch);
                    
                    int outputLength = strlen(value);

                    switch (strlen(value)) {
                        case 2:
                        case 3:
                        case 4:
                        case 7:
                            ++answer;
                            break;
                        default:
                            break;
                    }
                }
            }
        }

        fclose(inputFile);

        printf("%d", answer);
    }

    return 0;
}
