/* Day 19, part 1 = 459 */

#include "scanners.h"

int main() {
    struct ScannerData *data = getScannerData();

    if (data) {
        mapScanners(data);

        int answer = data->beacons->size;

        freeScannerData(data);

        printf("%d", answer);
    }

    return 0;
}
