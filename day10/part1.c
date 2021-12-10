/* Day 10, part 1 = 374061 */

#include "stack.h"

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        struct CharStack *stack = (struct CharStack *)calloc(1, sizeof(struct CharStack));
        int answer = 0;

        while (!feof(inputFile)) {
            char c = fgetc(inputFile);
            char opener;

            switch (c) {
                case '(':
                case '[':
                case '{':
                case '<':
                    push(stack, c);
                    break;
                case ')':
                case ']':
                case '}':
                case '>':
                    opener = pop(stack);

                    if (closer(opener) != c) {
                        switch (c) {
                            case ')':
                                answer += 3;
                                break;
                            case ']':
                                answer += 57;
                                break;
                            case '}':
                                answer += 1197;
                                break;
                            case '>':
                                answer += 25137;
                                break;
                        }
                    }

                    break;
                case '\n':
                    clear(stack);
                    break;
            }
        }

        fclose(inputFile);

        free(stack->data);
        free(stack);

        printf("%d", answer);
    }

    return 0;
}
