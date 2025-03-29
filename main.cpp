#include "game.h"

int main(int argc, char* argv[]) {
    Game game;

    if (!game.init()) {
        return -1;
    }

    game.run();
    game.close();

    return 0;
}
