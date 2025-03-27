#include "game.h"
#include <iostream>

int main(int argc, char* argv[]) {
    Game* game = new Game();

    if (!game->init()) {
        std::cerr << "Failed to initialize game!" << std::endl;
        delete game;
        return -1;
    }

    game->run();
    delete game;
    return 0;
}
