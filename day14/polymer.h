#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct PolymerPair {
    char left;
    char right;
    char insertion;
    long count;
};

struct PolymerData {
    struct PolymerPair *pairs;
    int pairCount;

    long elements[26];
};

void addPair(struct PolymerData *data, char left, char right, char insertion) {
    ++data->pairCount;
    data->pairs = (struct PolymerPair *)realloc(data->pairs, data->pairCount * sizeof(struct PolymerPair));

    data->pairs[data->pairCount - 1].left = left;
    data->pairs[data->pairCount - 1].right = right;
    data->pairs[data->pairCount - 1].insertion = insertion;
}

void incrementPair(struct PolymerData *data, char left, char right, long count) {
    for (int i = 0; i < data->pairCount; i++) {
        struct PolymerPair pair = data->pairs[i];

        if (pair.left == left && pair.right == right) {
            data->pairs[i].count += count;
            break;
        }
    }
}

long performSteps(struct PolymerData *data, int steps) {
    for (int i = 0; i < steps; i++) {
        struct PolymerPair *pairs = (struct PolymerPair *)calloc(data->pairCount, sizeof(struct PolymerPair));
        memcpy(pairs, data->pairs, data->pairCount * sizeof(struct PolymerPair));

        for (int j = 0; j < data->pairCount; j++) {
            if (pairs[j].count > 0) {
                data->elements[pairs[j].insertion - 'A'] += pairs[j].count;

                data->pairs[j].count -= pairs[j].count;

                incrementPair(data, pairs[j].left, pairs[j].insertion, pairs[j].count);
                incrementPair(data, pairs[j].insertion, pairs[j].right, pairs[j].count);
            }
        }

        free(pairs);
    }

    long elementCount;
    long maxElementCount = 0;
    long minElementCount = 10000000000000;

    for (int i = 0; i < 26; i++) {
        if ((elementCount = data->elements[i])) {
            if (elementCount < minElementCount) {
                minElementCount = elementCount;
            }

            if (elementCount > maxElementCount) {
                maxElementCount = elementCount;
            }
        }
    }

    return maxElementCount - minElementCount;
}

struct PolymerData *getPolymerData() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char left;
        char right;
        char insertion;
        char c;

        struct PolymerData *data = (struct PolymerData *)calloc(1, sizeof(struct PolymerData));

        while ((c = fgetc(inputFile)) != '\n');

        // \n
        fgetc(inputFile);

        while (fscanf(inputFile, "%c%c -> %c%c", &left, &right, &insertion, &c) > 0) {
            addPair(data, left, right, insertion);
        }

        fseek(inputFile, 0, SEEK_SET);

        left = '\0';
        right = '\0';

        while ((c = fgetc(inputFile)) != '\n') {
            ++data->elements[c - 'A'];

            right = c;

            if (left && right) {
                incrementPair(data, left, right, 1);
            }

            left = right;
        }

        fclose(inputFile);

        return data;
    }

    return NULL;
}

void freePolymerData(struct PolymerData *data) {
    free(data->pairs);
    free(data);
}
