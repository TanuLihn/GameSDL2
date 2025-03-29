#ifndef GAME_H
#define GAME_H

#include <SDL.h>
#include "background.h"
#include "box.h"

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
    Background* background;
    Box* box;

    int boxX, boxY;
    int boxSpeed;
    bool isFalling;
    bool direction;

    int grid[7][7];
    bool isRunning;
};

#endif
