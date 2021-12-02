/* Day 2, part 1 = 1660158 */

#include <stdio.h>
#include <string.h>

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        int position = 0;
        int depth = 0;

        char command[8];
        int units;

        while (!feof(inputFile) && fscanf(inputFile, "%s %d", command, &units)) {
            if (strcmp(command, "forward") == 0) {
                position += units;
            } else if (strcmp(command, "up") == 0) {
                depth -= units;
            } else if (strcmp(command, "down") == 0) {
                depth += units;
            }
        }

        fclose(inputFile);

        printf("%d", position * depth);
    }

    return 0;
}
