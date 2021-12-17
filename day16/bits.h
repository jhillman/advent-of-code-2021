#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum ExpressionPartType {
    SUM,
    PRODUCT,
    MINIMUM,
    MAXIMUM,
    VALUE,
    GREATER_THAN,
    LESS_THAN,
    EQUAL_TO
};

enum OperatorLengthType {
    SUB_PACKET_LENGTH,
    SUB_PACKET_COUNT
};

struct ExpressionPart {
    enum ExpressionPartType type;
    long value;

    struct ExpressionPart **parts;
    int partCount;
};

struct ExpressionPart *addPart(struct ExpressionPart *part) {
    if (part) {
        ++part->partCount;

        if (part->partCount == 0) {
            part->parts = (struct ExpressionPart **)malloc(sizeof(struct ExpressionPart *));
        } else {
            part->parts = (struct ExpressionPart **)realloc(part->parts, part->partCount * sizeof(struct ExpressionPart *));
        }

        struct ExpressionPart *newPart = (struct ExpressionPart *)calloc(1, sizeof(struct ExpressionPart));

        part->parts[part->partCount - 1] = newPart;

        return newPart;
    }

    return NULL;
}

void freeExpression(struct ExpressionPart *part) {
    for (int i = 0; i < part->partCount; i++) {
        freeExpression(part->parts[i]);
    }

    free(part);
}

long evaluate(struct ExpressionPart *part) {
    long result;

    switch (part->type) {
        case SUM:
            result = 0;

            for (int i = 0; i < part->partCount; i++) {
                result += evaluate(part->parts[i]);
            }
            break;
        case PRODUCT:
            result = 1;

            for (int i = 0; i < part->partCount; i++) {
                result *= evaluate(part->parts[i]);
            }
            break;
        case MINIMUM:
            for (int i = 0; i < part->partCount; i++) {
                long value = evaluate(part->parts[i]);

                if (i == 0 || value < result) {
                    result = value;
                }
            }
            break;
        case MAXIMUM:
            for (int i = 0; i < part->partCount; i++) {
                long value = evaluate(part->parts[i]);

                if (i == 0 || value > result) {
                    result = value;
                }
            }
            break;
        case VALUE:
            result = part->value;
            break;
        case GREATER_THAN:
            result = evaluate(part->parts[0]) > evaluate(part->parts[1]) ? 1 : 0;
            break;
        case LESS_THAN:
            result = evaluate(part->parts[0]) < evaluate(part->parts[1]) ? 1 : 0;
            break;
        case EQUAL_TO:
            result = evaluate(part->parts[0]) == evaluate(part->parts[1]) ? 1 : 0;
            break;     
        default:
            break;       
    }

    return result;
}

int readPacket(char *packet, int *packetLength, struct ExpressionPart *part);

long toLong(char *bits) {
    long value = 0;

    long bitValue = 1L << (strlen(bits) - 1);
    char *bit = bits;

    while (*bit) {
        value += *bit == '1' ? bitValue : 0;

        bitValue >>= 1;
        ++bit;
    }

    return value;
}

void readValue(char *packet, int *packetLength, struct ExpressionPart *part) {
    char *valuePart = packet;
    char valueBits[65] = "";

    *packetLength += 5;

    strncat(valueBits, valuePart + 1, 4);

    while (*valuePart != '0') {
        valuePart += 5;
        *packetLength += 5;

        strncat(valueBits, valuePart + 1, 4);
    }

    if (part) {
        part->value = toLong(valueBits);
    }
}

int readOperator(char *packet, int *packetLength, struct ExpressionPart *part) {
    enum OperatorLengthType lengthType = *packet - '0';
    int versionSum = 0;
    char subPacketInfoBits[16] = "";

    packet += 1;
    *packetLength += 1;

    switch (lengthType) {
        case SUB_PACKET_LENGTH:
            strncpy(subPacketInfoBits, packet, 15);

            long subPacketsLength = toLong(subPacketInfoBits);
        
            packet += 15;
            *packetLength += 15;

            int totalLength = 0;

            while (totalLength < subPacketsLength) {
                int subPacketlength = 0;

                struct ExpressionPart *operatorPart = addPart(part);

                versionSum += readPacket(packet, &subPacketlength, operatorPart);
                packet += subPacketlength;

                totalLength += subPacketlength;
            }
        
            *packetLength += subPacketsLength;            
            break;
        case SUB_PACKET_COUNT:
            strncpy(subPacketInfoBits, packet, 11);

            long subPacketsCount = toLong(subPacketInfoBits);

            packet += 11;
            *packetLength += 11;

            for (int i = 0; i < subPacketsCount; i++) {
                int subPacketlength = 0;

                struct ExpressionPart *operatorPart = addPart(part);

                versionSum += readPacket(packet, &subPacketlength, operatorPart);
    
                packet += subPacketlength;
                *packetLength += subPacketlength;
            }
            break;
    }

    return versionSum;
}

int readPacket(char *packet, int *packetLength, struct ExpressionPart *part) {
    char versionBits[4] = "";
    char typeBits[4] = "";
    int length = 0;
    int versionSum = 0;

    if (strchr(packet, '1')) {
        strncpy(versionBits, packet, 3);
        strncpy(typeBits, packet + 3, 3);

        packet += 6;

        if (packetLength) {
            *packetLength += 6;
        }

        int version = toLong(versionBits);
        enum ExpressionPartType type = toLong(typeBits);

        versionSum += version;

        if (part) {
            part->type = type;
        }

        switch (type) {
            case VALUE:
                readValue(packet, &length, part);

                if (packetLength) {
                    *packetLength += length;
                }
                break;
            default:
                versionSum += readOperator(packet, &length, part);

                if (packetLength) {
                    *packetLength += length;
                }
                break;
        }
    }

    return versionSum;
}

char *getTransmission() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char hexBits[][5] = {
            "0000",
            "0001",
            "0010",
            "0011",
            "0100",
            "0101",
            "0110",
            "0111",
            "1000",
            "1001",
            "1010",
            "1011",
            "1100",
            "1101",
            "1110",
            "1111"
        };
        char hex;
        int transmissionLength = 32;
        char *transmission = (char *)malloc(transmissionLength * sizeof(char));

        strcpy(transmission, "");

        while (!feof(inputFile) && (hex = fgetc(inputFile))) {
            int hexBitsIndex = hex > '9' ? 10 + hex - 'A' : hex - '0';

            if (hexBitsIndex >=0 && hexBitsIndex < 16) {
                if (strlen(transmission) + 4 > transmissionLength) {
                    transmissionLength += 1024;

                    transmission = (char *)realloc(transmission, transmissionLength * sizeof(char));
                }

                strcat(transmission, hexBits[hexBitsIndex]);
            }
        }

        fclose(inputFile);

        return transmission;
    }

    return NULL;
}