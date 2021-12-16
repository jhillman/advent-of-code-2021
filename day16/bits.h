#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum PacketType {
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

struct Expression {
    enum PacketType type;
    long value;
};

struct ExpressionStack {
    struct Expression *data;
    int size;
    int capacity;
};

void pushExpression(struct ExpressionStack *stack, struct Expression expression) {
    if (stack->capacity == 0) {
        stack->capacity = 5;
        stack->data = (struct Expression *)malloc(stack->capacity * sizeof(struct Expression));
    } else if (stack->size == stack->capacity) {
        stack->capacity += 5;
        stack->data = (struct Expression *)realloc(stack->data, stack->capacity * sizeof(struct Expression));
    }

    stack->data[stack->size++] = expression;
}

struct Expression popExpression(struct ExpressionStack *stack) {
    struct Expression expression = stack->data[stack->size - 1];

    --stack->size;

    return expression;
}

int readPacket(char *packet, int *packetLength);

int toInt(char *bits) {
    int value = 0;

    int bitValue = 1 << (strlen(bits) - 1);
    char *bit = bits;

    while (*bit) {
        value += *bit == '1' ? bitValue : 0;

        bitValue >>= 1;
        ++bit;
    }

    return value;
}

int readValue(char *packet, int *packetLength) {
    char *valuePart = packet;
    char valueBits[65] = "";

    *packetLength += 5;

    strncat(valueBits, valuePart + 1, 4);

    while (*valuePart != '0') {
        valuePart += 5;
        *packetLength += 5;

        strncat(valueBits, valuePart + 1, 4);
    }

    return toInt(valueBits);
}

int readOperator(char *packet, int *packetLength) {
    enum OperatorLengthType lengthType = *packet - '0';
    int versionSum = 0;
    char subPacketInfoBits[16] = "";

    packet += 1;
    *packetLength += 1;

    switch (lengthType) {
        case SUB_PACKET_LENGTH:
            strncpy(subPacketInfoBits, packet, 15);

            int subPacketsLength = toInt(subPacketInfoBits);
        
            packet += 15;
            *packetLength += 15;

            int totalLength = 0;

            while (totalLength < subPacketsLength) {
                int subPacketlength = 0;

                versionSum += readPacket(packet, &subPacketlength);
                packet += subPacketlength;

                totalLength += subPacketlength;
            }
        
            *packetLength += subPacketsLength;            
            break;
        case SUB_PACKET_COUNT:
            strncpy(subPacketInfoBits, packet, 11);

            int subPacketsCount = toInt(subPacketInfoBits);

            packet += 11;
            *packetLength += 11;

            for (int i = 0; i < subPacketsCount; i++) {
                int subPacketlength = 0;

                versionSum += readPacket(packet, &subPacketlength);
    
                packet += subPacketlength;
                *packetLength += subPacketlength;
            }
            break;
    }

    return versionSum;
}

int readPacket(char *packet, int *packetLength) {
    char versionBits[4] = "";
    char typeBits[4] = "";
    int length = 0;
    int versionSum = 0;

    if (strchr(packet, '1')) {
        strncpy(versionBits, packet, 3);
        strncpy(typeBits, packet + 3, 3);

        packet += 6;
        *packetLength += 6;

        int version = toInt(versionBits);
        enum PacketType type = toInt(typeBits);

        versionSum += version;

        switch (type) {
            case VALUE:
                readValue(packet, &length);

                *packetLength += length;
                break;
            default:
                versionSum += readOperator(packet, &length);

                *packetLength += length;
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
        int answer = 0;

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