#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

struct Room {
    int hallwayPosition;
    char amphipod;
    char amphipods[ROOM_DEPTH];
};

bool roomAvailable(struct Room room) {
    bool available = *room.amphipods == '.';

    for (int i = 1; available && i < ROOM_DEPTH; i++) {
        available = room.amphipods[i] == '.' || room.amphipods[i] == room.amphipod;
    }

    return available;
}

bool roomHasAmphipodsToMove(struct Room room) {
    for (int i = 0; i < ROOM_DEPTH; i++) {
        if (isalpha(room.amphipods[i]) && room.amphipods[i] != room.amphipod) {
            return true;
        }
    }

    return false;
}

struct Burrow {
    char hallway[11];
    struct Room aRoom;
    struct Room bRoom;
    struct Room cRoom;
    struct Room dRoom;
};

void printBurrow(struct Burrow burrow) {
    printf("#############\n");
    printf("#");

    for (int i = 0; i < 11; i++) {
        printf("%c", burrow.hallway[i]);
    }

    printf("#\n");

    for (int i = 0; i < ROOM_DEPTH; i++) {
        printf("%s#%c#%c#%c#%c#%s\n", i == 0 ? "##" : "  ", burrow.aRoom.amphipods[i], burrow.bRoom.amphipods[i], burrow.cRoom.amphipods[i], burrow.dRoom.amphipods[i], i == 0 ? "##" : "  ");
    }

    printf("  #########  \n\n");
}

bool burrowsEqual(struct Burrow first, struct Burrow second) {
    return strncmp(first.hallway, second.hallway, 11) == 0 &&
        strncmp(first.aRoom.amphipods, second.aRoom.amphipods, ROOM_DEPTH) == 0 &&
        strncmp(first.bRoom.amphipods, second.bRoom.amphipods, ROOM_DEPTH) == 0 &&
        strncmp(first.cRoom.amphipods, second.cRoom.amphipods, ROOM_DEPTH) == 0 &&
        strncmp(first.dRoom.amphipods, second.dRoom.amphipods, ROOM_DEPTH) == 0;
}

struct BurrowState {
    struct Burrow burrow;
    int energy;
};

int compareBurrows(struct Burrow first, struct Burrow second) {
    int result = result = strncmp(first.hallway, second.hallway, 11);

    if (result == 0) {
        result = strncmp(first.aRoom.amphipods, second.aRoom.amphipods, ROOM_DEPTH);
    }

    if (result == 0) {
        result = strncmp(first.bRoom.amphipods, second.bRoom.amphipods, ROOM_DEPTH);
    }

    if (result == 0) {
        result = strncmp(first.cRoom.amphipods, second.cRoom.amphipods, ROOM_DEPTH);
    }

    if (result == 0) {
        result = strncmp(first.dRoom.amphipods, second.dRoom.amphipods, ROOM_DEPTH);
    }

    return result;
}

int compare(const void *a, const void *b) {
    struct BurrowState *first = *(struct BurrowState **)a;
    struct BurrowState *second = *(struct BurrowState **)b;

    return compareBurrows(first->burrow, second->burrow);
}

void swap(struct BurrowState *a, struct BurrowState *b) {
    struct BurrowState temp = *b;

    *b = *a;
    *a = temp;
}   

struct BurrowStateQueue {
    struct BurrowState *data;
    int capacity;
    int size;
};

void heapify(struct BurrowStateQueue *queue, int index) {
    if (queue->size > 1) {
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < queue->size && queue->data[left].energy < queue->data[smallest].energy) {
            smallest = left;
        }

        if (right < queue->size && queue->data[right].energy < queue->data[smallest].energy) {
            smallest = right;
        }

        if (smallest != index) {
            swap(&queue->data[index], &queue->data[smallest]);
            heapify(queue, smallest);
        }
    }
}

void enqueueBurrowState(struct BurrowStateQueue *queue, struct BurrowState burrowState) {
    if (queue->capacity == 0) {
        queue->capacity = 1000;
        queue->data = (struct BurrowState *)malloc(queue->capacity * sizeof(struct BurrowState));
    } else if (queue->size == queue->capacity) {
        queue->capacity += 1000;
        queue->data = (struct BurrowState *)realloc(queue->data, queue->capacity * sizeof(struct BurrowState));
    }

    queue->data[queue->size++] = burrowState;

    for (int i = queue->size / 2 - 1; i >= 0; i--) {
        heapify(queue, i);
    }
}

struct BurrowState dequeueBurrowState(struct BurrowStateQueue *queue) {
    struct BurrowState burrowState = *queue->data;

    --queue->size;

    memcpy(queue->data, queue->data + 1, queue->size * sizeof(struct BurrowState));

    for (int i = queue->size / 2 - 1; i >= 0; i--) {
        heapify(queue, i);
    }

    return burrowState;
}

struct BurrowStateList {
    struct BurrowState **data;
    int capacity;
    int size;
};

struct BurrowState *binarySearchBurrowStates(struct BurrowState **burrowStates, struct Burrow burrow, int low, int high) {
    if (low > high) {
        return NULL;
    }

    int middle = low + ((high - low) / 2);

    if (burrowsEqual(burrow, burrowStates[middle]->burrow)) {
        return burrowStates[middle];
    } else if (compareBurrows(burrow, burrowStates[middle]->burrow) < 0) {
        return binarySearchBurrowStates(burrowStates, burrow, low, middle - 1);
    } else {
        return binarySearchBurrowStates(burrowStates, burrow, middle + 1, high);
    }
}

struct BurrowState *findBurrowState(struct BurrowStateList *burrowStates, struct Burrow burrow) {
    return binarySearchBurrowStates(burrowStates->data, burrow, 0, burrowStates->size - 1);
}

void addBurrowState(struct BurrowStateList *list, struct BurrowState *burrowState) {
    if (list->capacity == 0) {
        list->capacity = 1000;

        list->data = (struct BurrowState **)malloc(list->capacity * sizeof(struct BurrowState *));
    } else if (list->size == list->capacity) {
        list->capacity += 1000;

        list->data = (struct BurrowState **)realloc(list->data, list->capacity * sizeof(struct BurrowState *));
    }

    list->data[list->size++] = burrowState;

    qsort(list->data, list->size, sizeof(struct BurrowState *), compare);
}

bool burrowHallwayPathClear(struct Burrow burrow, int start, int end) {
    bool clear = true;
    char *space = burrow.hallway + start;

    if (start < end) {
        while (clear && ++space <= burrow.hallway + end) {
            clear = *space == '.';
        }
    } else {
        while (clear && --space >= burrow.hallway + end) {
            clear = *space == '.';
        }
    }

    return clear;
}

struct Room roomForAmphipod(struct Burrow burrow, char amphipod) {
    struct Room room;

    switch (amphipod) {
        case 'A':
            room = burrow.aRoom;
            break;
        case 'B':
            room = burrow.bRoom;
            break;
        case 'C':
            room = burrow.cRoom;
            break;
        case 'D':
            room = burrow.dRoom;
            break;
    }

    return room;
}

void updateRoom(struct Burrow *burrow, struct Room room) {
    switch (room.amphipod) {
        case 'A':
            burrow->aRoom = room;
            break;
        case 'B':
            burrow->bRoom = room;
            break;
        case 'C':
            burrow->cRoom = room;
            break;
        case 'D':
            burrow->dRoom = room;
            break;
    }
}

int organizeBurrow(struct Burrow original) {
    struct BurrowStateQueue *burrowStateQueue = (struct BurrowStateQueue *)calloc(1, sizeof(struct BurrowStateQueue *));
    struct Burrow organized = {
        { "..........." },
        { 2, 'A', "" },
        { 4, 'B', "" },
        { 6, 'C', "" },
        { 8, 'D', "" },
    };

    for (int i = 0; i < ROOM_DEPTH; i++) {
        organized.aRoom.amphipods[i] = 'A';
        organized.bRoom.amphipods[i] = 'B';
        organized.cRoom.amphipods[i] = 'C';
        organized.dRoom.amphipods[i] = 'D';
    }

    char amphipods[] = "ABCD";
    int amphipodEnergy[] = { 1, 10, 100, 1000 };

    struct BurrowStateList *burrowStateCosts = (struct BurrowStateList *)calloc(1, sizeof(struct BurrowStateList));

    int energy = 0;

    enqueueBurrowState(burrowStateQueue, (struct BurrowState){ original, 0 });

    while (burrowStateQueue->size) {
        struct BurrowState burrowState = dequeueBurrowState(burrowStateQueue);

        struct Burrow burrow = burrowState.burrow;

        if (burrowsEqual(burrow, organized)) {
            energy = burrowState.energy;
            break;
        }

        // move amphipods in hallway into their rooms
        for (int i = 0; i < 11; i++) {
            if (isalpha(burrow.hallway[i])) {
                char amphipod = burrow.hallway[i];
                struct Room room = roomForAmphipod(burrow, amphipod);

                if (roomAvailable(room) && burrowHallwayPathClear(burrow, i, room.hallwayPosition)) {
                    int roomPosition = strrchr(room.amphipods, '.') - room.amphipods;
                    int stepsIntoRoom = roomPosition + 1;
                    int energy = burrowState.energy + 
                        (abs(i - room.hallwayPosition) + stepsIntoRoom) * amphipodEnergy[amphipod - 'A'];

                    struct Burrow newBurrow = burrow;

                    newBurrow.hallway[i] = '.';

                    struct Room newRoom = room;
                    newRoom.amphipods[roomPosition] = newRoom.amphipod;

                    updateRoom(&newBurrow, newRoom);

                    struct BurrowState *newBurrowState = findBurrowState(burrowStateCosts, newBurrow);

                    if (newBurrowState == NULL) {
                        newBurrowState = (struct BurrowState *)calloc(1, sizeof(struct BurrowState));

                        newBurrowState->burrow = newBurrow;
                        newBurrowState->energy = energy;

                        addBurrowState(burrowStateCosts, newBurrowState);
                        enqueueBurrowState(burrowStateQueue, *newBurrowState);
                    } else if (energy < newBurrowState->energy) {
                        newBurrowState->energy = energy;

                        enqueueBurrowState(burrowStateQueue, *newBurrowState);
                    }
                }
            }
        }

        // move amphipods in wrong rooms into the hallway
        for (int i = 0; i < 4; i++) {
            struct Room room = roomForAmphipod(burrow, amphipods[i]);

            if (roomHasAmphipodsToMove(room)) {
                for (int j = 0; j < 11; j++) {
                    if ((j == 0 || j == 10 || j % 2 == 1) && burrow.hallway[j] == '.' && burrowHallwayPathClear(burrow, j, room.hallwayPosition)) {
                        char *roomPosition = room.amphipods;

                        while (*roomPosition == '.') {
                            ++roomPosition;
                        }

                        int amphipodPosition = roomPosition - room.amphipods;
                        char amphipod = *roomPosition;
                        int stepsOutOfRoom = amphipodPosition + 1;
                        int energy = burrowState.energy + 
                            (abs(room.hallwayPosition - j) + stepsOutOfRoom) * amphipodEnergy[amphipod - 'A'];

                        struct Burrow newBurrow = burrow;

                        newBurrow.hallway[j] = amphipod;

                        struct Room newRoom = room;
                        newRoom.amphipods[amphipodPosition] = '.';

                        updateRoom(&newBurrow, newRoom);

                        struct BurrowState *newBurrowState = findBurrowState(burrowStateCosts, newBurrow);

                        if (newBurrowState == NULL) {
                            newBurrowState = (struct BurrowState *)calloc(1, sizeof(struct BurrowState));

                            newBurrowState->burrow = newBurrow;
                            newBurrowState->energy = energy;

                            addBurrowState(burrowStateCosts, newBurrowState);
                            enqueueBurrowState(burrowStateQueue, *newBurrowState);
                        } else if (energy < newBurrowState->energy) {
                            newBurrowState->energy = energy;

                            enqueueBurrowState(burrowStateQueue, *newBurrowState);
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < burrowStateCosts->size; i++) {
        free(burrowStateCosts->data[i]);
    }

    free(burrowStateCosts);

    free(burrowStateQueue->data);
    free(burrowStateQueue);

    return energy;
}

bool readBurrow(char *filename, struct Burrow *burrow) {
    FILE *inputFile = fopen(filename, "r");

    if (inputFile) {
        char diagramLine[16];
        *burrow = (struct Burrow){
            { "..........." },
            { 2, 'A', "" },
            { 4, 'B', "" },
            { 6, 'C', "" },
            { 8, 'D', "" },
        };

        fgets(diagramLine, sizeof(diagramLine), inputFile);
        fgets(diagramLine, sizeof(diagramLine), inputFile);

        strncpy(burrow->hallway, diagramLine + 1, 11);

        for (int i = 0; i < ROOM_DEPTH; i++) {
            if (i == 0) {
                fscanf(inputFile, "###%c#%c#%c#%c###", &burrow->aRoom.amphipods[i], &burrow->bRoom.amphipods[i], &burrow->cRoom.amphipods[i], &burrow->dRoom.amphipods[i]);
            } else {
                fscanf(inputFile, "  #%c#%c#%c#%c#  ", &burrow->aRoom.amphipods[i], &burrow->bRoom.amphipods[i], &burrow->cRoom.amphipods[i], &burrow->dRoom.amphipods[i]);
            }
        }

        fclose(inputFile);

        return true;
    }

    return false;
}