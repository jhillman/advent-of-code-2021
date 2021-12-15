#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_LEVEL 1000000000

struct ChitonsData {
    int width;
    int height;

    char **riskLevels;
};

struct LocationLevel {
    int x;
    int y;
    int level;
};

struct LocationLevelQueue {
    struct LocationLevel *data;
    int capacity;
    int size;
};

bool equal(struct LocationLevel a, struct LocationLevel b) {
    return a.x == b.x && a.y == b.y && a.level == b.level;
}

int compare(const void *a, const void *b) {
    struct LocationLevel *aLevel = (struct LocationLevel *)a;
    struct LocationLevel *bLevel = (struct LocationLevel *)b;

    if (aLevel->level == bLevel->level) {
        if (aLevel->x != bLevel->x) {
            return bLevel->x - aLevel->x;
        } else {
            return bLevel->y - aLevel->y;
        }
    }

    return bLevel->level - aLevel->level;
}

void enqueue(struct LocationLevelQueue *queue, struct LocationLevel value) {
    if (queue->capacity == 0) {
        queue->capacity = 10;
        queue->data = (struct LocationLevel *)malloc(queue->capacity * sizeof(struct LocationLevel));
    } else if (queue->size == queue->capacity) {
        queue->capacity += 10;
        queue->data = (struct LocationLevel *)realloc(queue->data, queue->capacity * sizeof(struct LocationLevel));
    }

    queue->data[queue->size++] = value;
}

struct LocationLevel dequeue(struct LocationLevelQueue *queue) {
    qsort(queue->data, queue->size, sizeof(struct LocationLevel), compare);

    return queue->data[--queue->size];
}

void delete(struct LocationLevelQueue *queue, struct LocationLevel level) {
    for (int i = 0; i < queue->size; i++) {
        struct LocationLevel level = queue->data[i];

        if (equal(queue->data[i], level)) {
            for (int j = i + 1; j < queue->size; j++) {
                queue->data[j - 1] = queue->data[j];
            }

            --queue->size;
            break;
        }
    }
}

void clear(struct LocationLevelQueue *queue) {
    while(queue->size) {
        dequeue(queue);
    }    
}

int lowestRisk(struct ChitonsData *data) {
    int **riskLevels = (int **)malloc(data->height * sizeof(int *));

    for (int y = 0; y < data->height; y++) {
        riskLevels[y] = (int *)malloc(data->width * sizeof(int));

        for (int x = 0; x < data->width; x++) {
            riskLevels[y][x] = MAX_LEVEL;
        }
    }

    struct LocationLevelQueue *queue = (struct LocationLevelQueue *)calloc(1, sizeof(struct LocationLevel *));
    struct LocationLevel level = {0, 0, 0};

    int xDeltas[] = {-1, 0, 1, 0};
    int yDeltas[] = {0, 1, 0, -1};

    enqueue(queue, (struct LocationLevel){0, 0, 0});

    **riskLevels = 0;

    while (queue->size) {
        struct LocationLevel level = dequeue(queue);

        for (int i = 0; i < 4; i++) {
            int x = level.x + xDeltas[i];
            int y = level.y + yDeltas[i];

            if (!(x >= 0 && x < data->width && y >= 0 && y < data->height)) {
                continue;
            }

            if (riskLevels[y][x] > riskLevels[level.y][level.x] + data->riskLevels[y][x] - '0') {
                if (riskLevels[y][x] != MAX_LEVEL) {
                    delete(queue, (struct LocationLevel){x, y, riskLevels[y][x]});
                }

                riskLevels[y][x] = riskLevels[level.y][level.x] + data->riskLevels[y][x] - '0';

                enqueue(queue, (struct LocationLevel){x, y, riskLevels[y][x]});
            }
        }
    }

    int risk = riskLevels[data->height - 1][data->width - 1];

    for (int y = 0; y < data->height; y++) {
        free(riskLevels[y]);
    }

    free(riskLevels);

    free(queue);

    return risk;
}

struct ChitonsData *getChitonsData(int multiplier) {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        struct ChitonsData *data = (struct ChitonsData *)calloc(1, sizeof(struct ChitonsData));

        char c = fgetc(inputFile);

        while (!feof(inputFile)) {
            if (c == '\n') {
                ++data->height;
            }

            if (!data->height) {
                ++data->width;
            }

            c = fgetc(inputFile);
        }

        ++data->height;

        fseek(inputFile, 0, SEEK_SET);

        data->riskLevels = (char **)calloc(data->height * multiplier, sizeof(char *));

        for (int y = 0; y < data->height * multiplier; y++) {
            data->riskLevels[y] = (char *)calloc(data->width * multiplier, sizeof(char));

            for (int x = 0; x < data->width * multiplier; x++) {
                if (x < data->width) {
                    data->riskLevels[y][x] = fgetc(inputFile);
                }
            }

            if (y < data->height) {
                fgetc(inputFile);
            }
        }

        fclose(inputFile);

        data->height *= multiplier;
        data->width *= multiplier;

        return data;
    }

    return NULL;
}

void freeChitonsData(struct ChitonsData *data) {
    for (int y = 0; y < data->height; y++) {
        free(data->riskLevels[y]);
    }

    free(data->riskLevels);
    free(data);
}