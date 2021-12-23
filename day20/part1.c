/* Day 20, part 1 = ? */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void clearImage(char **image, int width, int height) {
    for (int y = 0; y < height; y++) {
        memset(image[y], '.', width);
    }
}

int main() {
    FILE *inputFile = fopen("input.txt", "r");

    if (inputFile) {
        char enhancementAlgorithm[516];
        int imageWidth = 0;
        int imageHeight = 0;
        int expandedImageWidth;
        int expandedImageHeight;
        int steps = 2;
        char **inputImage;
        char **outputImage;
        int c;
        int answer = 0;

        fgets(enhancementAlgorithm, sizeof(enhancementAlgorithm), inputFile);
        fgetc(inputFile); // \n

        c = fgetc(inputFile);

        while (!feof(inputFile)) {
            if (c == '\n') {
                ++imageHeight;
            }

            if (!imageHeight) {
                ++imageWidth;
            }

            c = fgetc(inputFile);

            if (feof(inputFile)) {
                ++imageHeight;
            }
        }

        fseek(inputFile, 0, SEEK_SET);

        expandedImageWidth = steps + imageWidth + steps;
        expandedImageHeight = steps + imageHeight + steps;

        inputImage = (char **)malloc(expandedImageHeight * sizeof(char *));

        for (int y = 0; y < steps; y++) {
            inputImage[y] = (char *)malloc(expandedImageWidth * sizeof(char));
            memset(inputImage[y], '.', expandedImageWidth);
        }

        for (int y = steps; y < steps + imageHeight; y++) {
            inputImage[y] = (char *)malloc(expandedImageWidth * sizeof(char));
            memset(inputImage[y], '.', expandedImageWidth);

            for (int x = steps; x < steps + imageWidth; x++) {
                inputImage[y][x] = fgetc(inputFile);
            }

            fgetc(inputFile);
        }

        for (int y = steps + imageHeight; y < expandedImageHeight; y++) {
            inputImage[y] = (char *)malloc(expandedImageWidth * sizeof(char));
            memset(inputImage[y], '.', expandedImageWidth);
        }

        fclose(inputFile);

        outputImage = (char **)malloc(expandedImageHeight * sizeof(char *));

        for (int y = 0; y < expandedImageHeight; y++) {
            outputImage[y] = (char *)malloc(expandedImageWidth * sizeof(char));
        }

        for (int i = 0; i < steps; i++) {
            clearImage(outputImage, expandedImageWidth, expandedImageHeight);

            int index = 0;
            int bit = 1 << 8;

            for (int y = 0; y < 3; y++) {
                for (int x = 0; x < 3; x++) {
                    index += inputImage[y][x] == '#' ? 1 : 0;
                    bit >>= 1;
                }
            }

            outputImage[y][x] = enhancementAlgorithm[index];

            if (i < steps - 1) {
                char **temp = inputImage;
                inputImage = outputImage;
                outputImage = temp;
            }
        }

        for (int y = 0; y < expandedImageHeight; y++) {
            for (int x = 0; x < expandedImageWidth; x++) {
                printf("%c", outputImage[y][x]);
            }

            printf("\n");
        }

        for (int y = 0; y < expandedImageHeight; y++) {
            free(inputImage[y]);
        }

        free(inputImage);

        printf("%d", answer);
    }

    return 0;
}
