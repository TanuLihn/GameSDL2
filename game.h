#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include <iostream>

class Game {
public:
    Game();
    ~Game();

    bool init();
    void run();
    void handleEvents();
    void update();
    void render();
    void close();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool isRunning;

    int grid[7][7];   // Lưới 7x7 để chứa các hộp
    int boxX;         // Vị trí X của hộp đang di chuyển
    int boxY;         // Vị trí Y của hộp đang rơi
    int boxSpeed;     // Tốc độ di chuyển ngang
    bool isFalling;   // Trạng thái rơi của hộp
    bool direction;   // 0 = trái, 1 = phải

    void dropBox();
};

#endif
