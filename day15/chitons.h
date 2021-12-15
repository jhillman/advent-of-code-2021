#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define MAX_LEVEL 1000000000

struct ChitonsData {
    int width;
    int height;

    char **riskLevels;
};

struct Location {
    int x;
    int y;
    int risk;
};

struct LocationQueue {
    struct Location *data;
    int capacity;
    int size;
};

bool equal(struct Location a, struct Location b) {
    return a.x == b.x && a.y == b.y;
}

int compare(const void *a, const void *b) {
    struct Location *aLevel = (struct Location *)a;
    struct Location *bLevel = (struct Location *)b;

    return bLevel->risk - aLevel->risk;
}

void enqueue(struct LocationQueue *queue, struct Location location) {
    if (queue->capacity == 0) {
        queue->capacity = 1000;
        queue->data = (struct Location *)malloc(queue->capacity * sizeof(struct Location));
    } else if (queue->size == queue->capacity) {
        queue->capacity += 1000;
        queue->data = (struct Location *)realloc(queue->data, queue->capacity * sizeof(struct Location));
    }

    queue->data[queue->size++] = location;
}

struct Location dequeue(struct LocationQueue *queue) {
    qsort(queue->data, queue->size, sizeof(struct Location), compare);

    return queue->data[--queue->size];
}

void delete(struct LocationQueue *queue, struct Location location) {
    for (int i = 0; i < queue->size; i++) {
        struct Location location = queue->data[i];

        if (equal(queue->data[i], location)) {
            for (int j = i + 1; j < queue->size; j++) {
                queue->data[j - 1] = queue->data[j];
            }

            --queue->size;
            break;
        }
    }
}

void clear(struct LocationQueue *queue) {
    while(queue->size) {
        dequeue(queue);
    }    
}

int lowestRisk(struct ChitonsData *data) {
    int **cumulativeRiskLevels = (int **)malloc(data->height * sizeof(int *));

    for (int y = 0; y < data->height; y++) {
        cumulativeRiskLevels[y] = (int *)calloc(data->width, sizeof(int));

        for (int x = 0; x < data->width; x++) {
            cumulativeRiskLevels[y][x] = MAX_LEVEL;
        }
    }

    struct LocationQueue *queue = (struct LocationQueue *)calloc(1, sizeof(struct Location *));
    struct Location end = {data->width - 1, data->height - 1, 0};

    int xDeltas[] = {-1, 0, 1, 0};
    int yDeltas[] = {0, 1, 0, -1};

    enqueue(queue, (struct Location){0, 0, 0});

    **cumulativeRiskLevels = 0;

    while (queue->size) {
        struct Location location = dequeue(queue);

        if (equal(location, end)) {
            break;
        }

        for (int i = 0; i < 4; i++) {
            int x = location.x + xDeltas[i];
            int y = location.y + yDeltas[i];

            if (!(x >= 0 && x < data->width && y >= 0 && y < data->height)) {
                continue;
            }

            int risk = cumulativeRiskLevels[location.y][location.x] + data->riskLevels[y][x] - '0';

            if (cumulativeRiskLevels[y][x] > risk) {
                if (cumulativeRiskLevels[y][x] != MAX_LEVEL) {
                    delete(queue, (struct Location){x, y, cumulativeRiskLevels[y][x]});
                }

                cumulativeRiskLevels[y][x] = risk;

                enqueue(queue, (struct Location){x, y, risk});
            }
        }
    }

    int risk = cumulativeRiskLevels[end.y][end.x];

    for (int y = 0; y < data->height; y++) {
        free(cumulativeRiskLevels[y]);
    }

    free(cumulativeRiskLevels);

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