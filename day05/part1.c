/* Day 5, part 1 = 5280 */

#include "diagram.h"

int main() {
    struct DiagramData *data = getDiagramData();

    if (data) {
        int answer = updateDiagram(data, false);

        freeDiagramData(data);

        printf("%d", answer);
    }

    return 0;
}
