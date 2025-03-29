#include "win_level.h"

bool checkGameOver(int grid[7][7]) {
    // Kiểm tra xem có cột nào đã đầy (tất cả 7 ô) chưa
    for (int col = 0; col < 7; col++) {
        bool fullColumn = true;
        for (int row = 0; row < 7; row++) {
            if (grid[row][col] == 0) {
                fullColumn = false;
                break;
            }
        }
        if (fullColumn) {
            return true;
        }
    }

    // Kiểm tra xem có hàng nào đã đầy (tất cả 7 ô) chưa
    for (int row = 0; row < 7; row++) {
        bool fullRow = true;
        for (int col = 0; col < 7; col++) {
            if (grid[row][col] == 0) {
                fullRow = false;
                break;
            }
        }
        if (fullRow) {
            return true;
        }
    }

    // Nếu không có điều kiện thắng nào được đáp ứng, trò chơi tiếp tục
    return false;
}
