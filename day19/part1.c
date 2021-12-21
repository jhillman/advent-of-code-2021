/* Day 19, part 1 = 459 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define EMPTY -1000

struct Coordinates {
    int x;
    int y;
    int z;
};

struct Coordinates vector(struct Coordinates first, struct Coordinates second) {
    return (struct Coordinates){ second.x - first.x, second.y - first.y, second.z - first.z };
}

bool equal(struct Coordinates first, struct Coordinates second) {
    return first.x == second.x && first.y == second.y && first.z == second.z;
}

struct Coordinates empty() {
    return (struct Coordinates){ EMPTY, EMPTY, EMPTY };
}

bool isEmpty(struct Coordinates coordinates) {
    return coordinates.x == EMPTY && coordinates.y == EMPTY && coordinates.z == EMPTY;
}

struct Coordinates translate(struct Coordinates coordinates, struct Coordinates translation) {
    return (struct Coordinates){ coordinates.x + translation.x, coordinates.y + translation.y, coordinates.z + translation.z };
}

bool lessThan(struct Coordinates first, struct Coordinates second) {
    if (first.x == second.x && first.y == second.y) {
        return first.z < second.z;
    } else if (first.x == second.x) {
        return first.y < second.y;
    } else {
        return first.x < second.x;
    }
}

int compareCoordinates(const void *a, const void *b) {
    return lessThan(*(struct Coordinates *)a, *(struct Coordinates *)b) ? -1 : 1;
}

struct CoordinatesList {
    struct Coordinates *list;
    int capacity;
    int size;
};

int indexOfCoordinates(struct CoordinatesList *coordinatesList, struct Coordinates coordinates) {
    int index = -1;

    for (int i = 0; index == -1 && i < coordinatesList->size; i++) {
        if(equal(coordinates, coordinatesList->list[i])) {
            index = i;
        }
    }

    return index;    
}

void sortCoordinates(struct CoordinatesList *coordinatesList) {
    qsort(coordinatesList->list, coordinatesList->size, sizeof(struct Coordinates), compareCoordinates);    
}

void addCoordinates(struct CoordinatesList *coordinatesList, struct Coordinates coordinates, bool sort) {
    if (coordinatesList->capacity == 0) {
        coordinatesList->capacity = 500;
        coordinatesList->list = (struct Coordinates *)malloc(+coordinatesList->capacity * sizeof(struct Coordinates));
    } else {
        coordinatesList->capacity += 500;
        coordinatesList->list = (struct Coordinates *)realloc(coordinatesList->list, coordinatesList->capacity * sizeof(struct Coordinates));
    }

    coordinatesList->list[coordinatesList->size++] = coordinates;

    if (sort) {
        sortCoordinates(coordinatesList);
    }
}

struct VectorBeacon {
    struct Coordinates vector;
    struct Coordinates beacon;
};

int compareVectorBeacons(const void *a, const void *b) {
    return lessThan((*(struct VectorBeacon *)a).vector, (*(struct VectorBeacon *)b).vector) ? -1 : 1;
}

struct VectorBeaconList {
    struct VectorBeacon *list;
    int capacity;
    int size;
};

int binarySearchVectors(struct VectorBeacon *vectors, struct Coordinates vector, int low, int high) {
    if (low > high) {
        return -1;
    }

    int middle = low + ((high - low) / 2);

    if (equal(vectors[middle].vector, vector)) {
        return middle;
    } else if (lessThan(vector, vectors[middle].vector)) {
        return binarySearchVectors(vectors, vector, low, middle - 1);
    } else {
        return binarySearchVectors(vectors, vector, middle + 1, high);
    }
}

int indexOfVector(struct VectorBeaconList *vectorBeaconsList, struct Coordinates vector) {
    return binarySearchVectors(vectorBeaconsList->list, vector, 0, vectorBeaconsList->size - 1);
}

void sortVectors(struct VectorBeaconList *vectorBeaconsList) {
    qsort(vectorBeaconsList->list, vectorBeaconsList->size, sizeof(struct VectorBeacon), compareVectorBeacons);    
}

void addVector(struct VectorBeaconList *vectorBeaconsList, struct VectorBeacon vectorBeacons, bool sort) {
    if (vectorBeaconsList->capacity == 0) {
        vectorBeaconsList->capacity = 1000;
        vectorBeaconsList->list = (struct VectorBeacon *)malloc(+vectorBeaconsList->capacity * sizeof(struct VectorBeacon));
    } else {
        vectorBeaconsList->capacity += 1000;
        vectorBeaconsList->list = (struct VectorBeacon *)realloc(vectorBeaconsList->list, vectorBeaconsList->capacity * sizeof(struct VectorBeacon));
    }

    vectorBeaconsList->list[vectorBeaconsList->size++] = vectorBeacons;

    if (sort) {
        sortVectors(vectorBeaconsList);
    }
}

struct Scanner {
    int id;

    struct Coordinates *readings;
    int readingCount;

    struct Coordinates **rotations;
    int rotationIndex;

    struct Coordinates translation;
    struct VectorBeaconList *vectors;

    bool *checkedScanners;
    bool established;
};

void addReading(struct Scanner *scanner, struct Coordinates reading) {
    if (scanner->readingCount == 0) {
        scanner->readings = (struct Coordinates *)malloc(++scanner->readingCount * sizeof(struct Coordinates));
    } else {
        scanner->readings = (struct Coordinates *)realloc(scanner->readings, ++scanner->readingCount * sizeof(struct Coordinates));
    }

    scanner->readings[scanner->readingCount - 1] = reading;
}

void updateVectors(struct Scanner *scanner) {
    scanner->vectors = (struct VectorBeaconList *)calloc(1, sizeof(struct VectorBeaconList));

    for (int i = 0; i < scanner->readingCount; i++) {
        for (int j = 0; j < scanner->readingCount; j++) {
            if (i != j) {
                struct VectorBeacon vectorBeacon = { vector(scanner->readings[j], scanner->readings[i]), scanner->readings[j] };

                if (indexOfVector(scanner->vectors, vectorBeacon.vector) == -1) {
                    addVector(scanner->vectors, vectorBeacon, false);
                }
            }
        }
    }

    sortVectors(scanner->vectors);
}

struct Coordinates rotate(struct Coordinates coordinates, int rotationIndex) {
    struct Coordinates rotated;

    switch (rotationIndex) {
        case 0: 
            rotated = (struct Coordinates){ coordinates.x, coordinates.y, coordinates.z };
            break;
        case 1: 
            rotated = (struct Coordinates){ coordinates.x, -coordinates.z, coordinates.y };
            break;
        case 2: 
            rotated = (struct Coordinates){ coordinates.x, -coordinates.y, -coordinates.z };
            break;
        case 3: 
            rotated = (struct Coordinates){ coordinates.x, coordinates.z, -coordinates.y };
            break;
        case 4: 
            rotated = (struct Coordinates){ -coordinates.y, coordinates.x, coordinates.z };
            break;
        case 5: 
            rotated = (struct Coordinates){ coordinates.z, coordinates.x, coordinates.y };
            break;
        case 6: 
            rotated = (struct Coordinates){ coordinates.y, coordinates.x, -coordinates.z };
            break;
        case 7: 
            rotated = (struct Coordinates){ -coordinates.z, coordinates.x, -coordinates.y };
            break;
        case 8: 
            rotated = (struct Coordinates){ -coordinates.x, -coordinates.y, coordinates.z };
            break;
        case 9: 
            rotated = (struct Coordinates){ -coordinates.x, -coordinates.z, -coordinates.y };
            break;
        case 10: 
            rotated = (struct Coordinates){ -coordinates.x, coordinates.y, -coordinates.z };
            break;
        case 11: 
            rotated = (struct Coordinates){ -coordinates.x, coordinates.z, coordinates.y };
            break;
        case 12: 
            rotated = (struct Coordinates){ coordinates.y, -coordinates.x, coordinates.z };
            break;
        case 13: 
            rotated = (struct Coordinates){ coordinates.z, -coordinates.x, -coordinates.y };
            break;
        case 14: 
            rotated = (struct Coordinates){ -coordinates.y, -coordinates.x, -coordinates.z };
            break;
        case 15: 
            rotated = (struct Coordinates){ -coordinates.z, -coordinates.x, coordinates.y };
            break;
        case 16: 
            rotated = (struct Coordinates){ -coordinates.z, coordinates.y, coordinates.x };
            break;
        case 17: 
            rotated = (struct Coordinates){ coordinates.y, coordinates.z, coordinates.x };
            break;
        case 18: 
            rotated = (struct Coordinates){ coordinates.z, -coordinates.y, coordinates.x };
            break;
        case 19: 
            rotated = (struct Coordinates){ -coordinates.y, -coordinates.z, coordinates.x };
            break;
        case 20: 
            rotated = (struct Coordinates){ -coordinates.z, -coordinates.y, -coordinates.x };
            break;
        case 21: 
            rotated = (struct Coordinates){ -coordinates.y, coordinates.z, -coordinates.x };
            break;
        case 22: 
            rotated = (struct Coordinates){ coordinates.z, coordinates.y, -coordinates.x };
            break;
        case 23: 
            rotated = (struct Coordinates){ coordinates.y, -coordinates.z, -coordinates.x };
            break;
    }

    return rotated;
}

void findTranslation(struct Scanner *establishedScanner, struct Scanner *scanner) {
    int matchCount = 0;
    bool translationFound = false;

    for (int i = 0; !translationFound && i < 24; i++) {
        for (int j = 0; !translationFound && j < scanner->readingCount; j++) {
            for (int k = 0; !translationFound && k < scanner->readingCount; k++) {
                if (k != j) {
                    int vectorIndex = indexOfVector(establishedScanner->vectors, vector(scanner->rotations[i][k], scanner->rotations[i][j]));

                    if (vectorIndex != -1 && ++matchCount == 11) {
                        scanner->rotationIndex = i;
                        scanner->translation = vector(scanner->rotations[i][k], establishedScanner->vectors->list[vectorIndex].beacon);

                        translationFound = true;
                    }
                }
            }
        }
    }
};

void translateReadings(struct Scanner *scanner) {
    for (int i = 0; i < scanner->readingCount; i++) {
        scanner->readings[i] = translate(scanner->rotations[scanner->rotationIndex][i], scanner->translation);
    }
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char line[20];
        int scannerCount = 0;
        struct Scanner **scanners = NULL;
        struct Scanner **scanner = NULL;
        int answer = 0;

        while (fgets(line, sizeof(line), inputFile)) {
            if (strstr(line, "---")) {
                ++scannerCount;
            }
        }

        scanners = (struct Scanner **)calloc(scannerCount, sizeof(struct Scanner *));

        fseek(inputFile, 0, SEEK_SET);

        while (fgets(line, sizeof(line), inputFile)) {
            if (strstr(line, "---")) {
                scanner = scanner ? scanner + 1 : scanners;
                *scanner = (struct Scanner *)calloc(1, sizeof(struct Scanner));
            } else if (strlen(line) == 1) {
                continue;
            } else {
                struct Coordinates reading;
                sscanf(line, "%d,%d,%d", &reading.x, &reading.y, &reading.z);

                addReading(*scanner, reading);
            }
        }

        for (int i = 0; i < scannerCount; i++) {
            scanners[i]->id = i;
            scanners[i]->rotationIndex = -1;
            scanners[i]->translation = empty();

            scanners[i]->rotations = (struct Coordinates **)malloc(24 * sizeof(struct Coordinates *));

            for (int j = 0; j < 24; j++) {
                scanners[i]->rotations[j] = (struct Coordinates *)malloc(scanners[i]->readingCount * sizeof(struct Coordinates));

                for (int k = 0; k < scanners[i]->readingCount; k++) {
                    scanners[i]->rotations[j][k] = rotate(scanners[i]->readings[k], j);
                }
            }  

            scanners[i]->checkedScanners = (bool *)calloc(scannerCount, sizeof(bool));
        }

        struct Scanner *establishedScanner = scanners[0];
        establishedScanner->established = true;
        establishedScanner->rotationIndex = 0;

        updateVectors(establishedScanner);

        struct CoordinatesList *beacons = (struct CoordinatesList *)calloc(1, sizeof(struct CoordinatesList));

        for (int i = 0; i < establishedScanner->readingCount; i++) {
            addCoordinates(beacons, establishedScanner->readings[i], false);
        }

        sortCoordinates(beacons);

        int matchingScannerCount = 1;
        int currentEstablishedScannerId;

        while (matchingScannerCount < scannerCount) {
            currentEstablishedScannerId = establishedScanner->id;

            for (int i = 1; i < scannerCount; i++) {
                if (establishedScanner->id == i || establishedScanner->checkedScanners[i] || scanners[i]->checkedScanners[establishedScanner->id] || scanners[i]->established) {
                    continue;
                }

                establishedScanner->checkedScanners[i] = true;
                scanners[i]->checkedScanners[establishedScanner->id] = true;

                findTranslation(establishedScanner, scanners[i]);

                if (!isEmpty(scanners[i]->translation)) {
                    scanners[i]->established = true;

                    ++matchingScannerCount;

                    translateReadings(scanners[i]);

                    for (int j = 0; j < scanners[i]->readingCount; j++) {
                        if (indexOfCoordinates(beacons, scanners[i]->readings[j]) == -1) {
                            addCoordinates(beacons, scanners[i]->readings[j], true);
                        }
                    }

                    updateVectors(scanners[i]);

                    establishedScanner = scanners[i];

                    break;
                }
            }

            if (matchingScannerCount < scannerCount && establishedScanner->id == currentEstablishedScannerId) {
                int index = establishedScanner->id;

                do {
                    ++index;

                    if (index > scannerCount - 1) {
                        index = 0;
                    }

                    establishedScanner = scanners[index];
                } while (!establishedScanner->established);
            } 
        }

        answer = beacons->size;

        free(beacons->list);
        free(beacons);

        for (int i = 0; i < scannerCount; i++) {
            if (scanners[i]) {
                free(scanners[i]->readings);

                for (int j = 0; j < 24; j++) {
                    free(scanners[i]->rotations[j]);
                }

                free(scanners[i]->rotations);

                free(scanners[i]->vectors->list);
                free(scanners[i]->vectors);

                free(scanners[i]->checkedScanners);

                free(scanners[i]);
            }
        }

        free(scanners);

        fclose(inputFile);

        printf("%d", answer);
    }

    return 0;
}
