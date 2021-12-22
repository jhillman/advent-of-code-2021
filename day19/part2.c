/* Day 19, part 2 = 19130 */

#include "scanners.h"

int main() {
    struct ScannerData *data = getScannerData();

    if (data) {
        mapScanners(data);

        int answer = 0;

        for (int i = 0; i < data->scannerCount; i++) {
            for (int j = i + 1; j < data->scannerCount; j++) {
                int scannerDistance = distance(data->scanners[i]->translation, data->scanners[j]->translation);

                if (scannerDistance > answer) {
                    answer = scannerDistance;
                }
            }
        }

        freeScannerData(data);

        printf("%d", answer);
    }

    return 0;
}
