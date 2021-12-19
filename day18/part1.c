/* Day 18, part 1 = 3816 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <math.h>

struct SnailfishNumber {
    struct SnailfishNumber *parent;

    long left;
    long right;
};

bool isSnailfishNumber(long number) {
    return number < 0;
}

bool isRegularNumber(long number) {
    return number >= 0;
}

struct SnailfishNumber *toSnailfishNumber(long numberAddress) {
    return (struct SnailfishNumber *)-numberAddress;
}

struct SnailfishNumber *readNumber(struct SnailfishNumber *parent, char *numberString, int *length) {
    struct SnailfishNumber *number = (struct SnailfishNumber *)calloc(1, sizeof(struct SnailfishNumber));
    number->parent = parent;

    if (length) {
        *length += 1; // [
    }

    int leftLength = 0;
    char *leftNumberString = numberString + 1;

    if (isdigit(*leftNumberString)) {
        number->left = *leftNumberString - '0';
        leftLength = 1;
    } else {
        number->left = -(long)readNumber(number, leftNumberString, &leftLength);
    }

    if (length) {
        *length += leftLength;
        *length += 1;  // ,
    }

    int rightLength = 0;
    char *rightNumberString = numberString + 1 + leftLength + 1;

    if (isdigit(*rightNumberString)) {
        number->right = *rightNumberString - '0';
        rightLength = 1;
    } else {
        number->right = -(long)readNumber(number, rightNumberString, &rightLength);
    }    

    if (length) {
        *length += rightLength;
        *length += 1;  // ]
    }

    return number;
}

void printNumber(struct SnailfishNumber *number, int depth) {
    printf("[");

    if (isSnailfishNumber(number->left)) {
        printNumber(toSnailfishNumber(number->left), depth + 1);
    } else {
        printf("%ld", number->left);
    }

    printf(",");

    if (isSnailfishNumber(number->right)) {
        printNumber(toSnailfishNumber(number->right), depth + 1);
    } else {
        printf("%ld", number->right);
    }

    printf("]%s", depth == 0 ? "\n" : "");
}

void freeNumber(struct SnailfishNumber *number) {
    if (isSnailfishNumber(number->left)) {
        freeNumber(toSnailfishNumber(number->left));
    }

    if (isSnailfishNumber(number->right)) {
        freeNumber(toSnailfishNumber(number->right));
    }

    free(number);
}

struct SnailfishNumber *add(struct SnailfishNumber *number, struct SnailfishNumber *addend) {
    printf("after addition: ");

    struct SnailfishNumber *sum = (struct SnailfishNumber *)calloc(1, sizeof(struct SnailfishNumber));

    number->parent = sum;
    addend->parent = sum;

    sum->left = -(long)number;
    sum->right = -(long)addend;

    return sum;
}

enum FindDirection {
    UP,
    DOWN
};

struct SnailfishNumber *findLeft(struct SnailfishNumber *number, struct SnailfishNumber* previous, enum FindDirection direction) {
    struct SnailfishNumber *left = NULL;

    printf("finding left %s: ", direction == UP ? "up" : "down");
    printNumber(number, 0);

    if (direction == UP && isRegularNumber(number->left)) {
        left = number;
    } else if (direction == DOWN && isRegularNumber(number->right)) {
        left = number;
    } else {
        if (direction == UP) {
            if (isSnailfishNumber(number->left) && number->left != -(long)previous) {
                left = findLeft(toSnailfishNumber(number->left), number, DOWN);
            } else if (number->parent) {
                left = findLeft(number->parent, number, UP);
            }
        } else {
            if (isSnailfishNumber(number->right)) {
                left = findLeft(toSnailfishNumber(number->right), number, DOWN);
            }
        }
    }

    return left;
}

// [                                   ]
//  [               ],[               ]
//   3,[           ]   6,[           ]
//      2,[       ]       5,[       ]
//         1,[   ]           4,[   ]
//            7,3               3,2

struct SnailfishNumber *findRight(struct SnailfishNumber *number, struct SnailfishNumber* previous, enum FindDirection direction) {
    struct SnailfishNumber *right = NULL;

    printf("finding right %s: ", direction == UP ? "up" : "down");
    printNumber(number, 0);

    if (direction == UP && isRegularNumber(number->right)) {
        right = number;
    } else if (direction == DOWN && isRegularNumber(number->left)) {
        right = number;
    } else {
        if (direction == UP) {
            if (isSnailfishNumber(number->right) && number->right != -(long)previous) {
                right = findRight(toSnailfishNumber(number->right), number, DOWN);
            } else if (number->parent) {
                right = findRight(number->parent, number, UP);
            }
        } else {
            if (isSnailfishNumber(number->left)) {
                right = findRight(toSnailfishNumber(number->left), number, DOWN);
            }
        }
    }

    return right;
}

void explodeNumber(struct SnailfishNumber *number) {
    printf("exploding: ");
    printNumber(number, 0);

    struct SnailfishNumber *left = findLeft(number->parent, number, UP);
    struct SnailfishNumber *right = findRight(number->parent, number, UP);

    if (left) {
        printf("left: ");
        printNumber(left, 0);

        if (isRegularNumber(left->right)) {
            left->right += number->left;
        } else {
            left->left += number->left;
        }
    }

    if (right) {
        printf("right: ");
        printNumber(right, 0);

        if (isRegularNumber(right->left)) {
            right->left += number->right;
        } else {
            right->right += number->right;
        }
    }

    printf("after explode:  ");
}

bool explode(struct SnailfishNumber *number, int depth) {
    bool success = false;

    if (isSnailfishNumber(number->left)) {
        if (depth == 3) {
            explodeNumber(toSnailfishNumber(number->left));

            free(toSnailfishNumber(number->left));
            
            number->left = 0;

            success = true;
        } else {
            success = explode(toSnailfishNumber(number->left), depth + 1);
        }
    }

    if (!success && isSnailfishNumber(number->right)) {
        if (depth == 3) {
            explodeNumber(toSnailfishNumber(number->right));

            free(toSnailfishNumber(number->right));

            number->right = 0;

            success = true;
        } else {
            success = explode(toSnailfishNumber(number->right), depth + 1);
        }
    }

    return success;
}

struct SnailfishNumber *splitNumber(struct SnailfishNumber *parent, long number) {
    printf("after split:    ");

    struct SnailfishNumber *split = (struct SnailfishNumber *)calloc(1, sizeof(struct SnailfishNumber));

    split->left = (long)floor((float)number / 2);
    split->right = (long)ceil((float)number / 2);
    split->parent = parent;

    return split;    
}

bool split(struct SnailfishNumber *number) {
    bool success = false;

    if (number->left > 9) {
        number->left = -(long)splitNumber(number, number->left);
        success = true;
    } else if (isSnailfishNumber(number->left)) {
        success = split(toSnailfishNumber(number->left));
    }

    if (!success && number->right > 9) {
        number->right = -(long)splitNumber(number, number->right);
        success = true;
    } else if (!success && isSnailfishNumber(number->right)) {
        success = split(toSnailfishNumber(number->right));
    }

    return success;   
}

void reduce(struct SnailfishNumber *number) {
    while (explode(number, 0) || split(number)) {
        printNumber(number, 0);
    }
}

long magnitude(struct SnailfishNumber *number) {
    long left = isSnailfishNumber(number->left) ? magnitude(toSnailfishNumber(number->left)) : number->left;
    long right = isSnailfishNumber(number->right) ? magnitude(toSnailfishNumber(number->right)) : number->right;

    return left * 3 + right * 2;
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char numberString[64];
        struct SnailfishNumber *number = NULL;

        while (fgets(numberString, sizeof(numberString), inputFile)) {
            if (number) {
                number = add(number, readNumber(NULL, numberString, NULL));

                printNumber(number, 0);

                reduce(number);
            } else {
                number = readNumber(NULL, numberString, NULL);

                // printNumber(number, 0);

                // explode(number, 0);
                // printNumber(number, 0);

                // explode(number, 0);
                // printNumber(number, 0);
            }
        }

        printNumber(number, 0);

        fclose(inputFile);

        long answer = magnitude(number);

        freeNumber(number);

        printf("%ld", answer);
    }

    return 0;
}
