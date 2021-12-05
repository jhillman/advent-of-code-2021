/* Day 5, part 2 = 16716 */

#include "diagram.h"

int main() {
    struct DiagramData *data = getDiagramData();

    if (data) {
        int answer = updateDiagram(data, true);

        freeDiagramData(data);

        printf("%d", answer);
    }

    return 0;
}
