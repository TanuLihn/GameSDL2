#include "win_level.h"

bool checkGameOver(int grid[7][7]) {
    int countFullColumns = 0;

    for (int col = 0; col < 7; col++) {
        int filled = 0;
        for (int row = 0; row < 7; row++) {
            if (grid[row][col] == 1) {
                filled++;
            }
        }
        if (filled == 7) {
            countFullColumns++;
        }
    }

    return countFullColumns >= 1; // Thắng khi có ít nhất 2 cột đầy
}

