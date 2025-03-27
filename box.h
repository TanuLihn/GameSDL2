#ifndef BOX_H
#define BOX_H

#include <SDL.h>

class Box {
public:
    Box(int x, int y, int size, SDL_Renderer* renderer);
    void update();  // Cập nhật vị trí hộp
    void render();  // Vẽ hộp lên màn hình
    void drop();    // Khi nhấn SPACE, hộp sẽ rơi xuống
    bool isFalling() const { return falling; }
    int getX() const { return x; }
    int getY() const { return y; }

private:
    int x, y;              // Vị trí của hộp
    int size;              // Kích thước hộp
    SDL_Renderer* renderer; // Renderer để vẽ hộp
    bool falling;          // Trạng thái rơi hay không
    int velocityY;         // Tốc độ rơi
};

#endif
