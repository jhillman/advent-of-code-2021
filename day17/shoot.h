#include <stdio.h>
#include <stdbool.h>

struct ShotResult {
    bool hitTarget;
    int maxHeight;
};

struct ShotResult shoot(int velocityX, int velocityY, int targetMinX, int targetMaxX, int targetMinY, int targetMaxY) {
    int x = 0;
    int y = 0;
    struct ShotResult result = { false, 0 };

    for (int i = 0; i < 500; i++) {
        x += velocityX;
        y += velocityY;

        if (velocityX > 0) {
            --velocityX;
        }

        if (velocityX < 0) {
            ++velocityX;
        }

        --velocityY;

        if (y > result.maxHeight) {
            result.maxHeight = y;
        }

        if (x >= targetMinX && x <= targetMaxX && y >= targetMinY && y <= targetMaxY) {
            result.hitTarget = true;
            break;
        }
    }

    return result;
}