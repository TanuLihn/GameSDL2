#include "highscore.h"
#include <iostream>
#include <fstream>

HighScore::HighScore() {
    loadHighScore();
}

void HighScore::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> bestTime;
        file.close();
    } else {
        bestTime = -1;
    }
}

void HighScore::saveHighScore(int time) {
    if (bestTime == -1 || time < bestTime) {
        bestTime = time;
        std::ofstream file("highscore.txt");
        if (file.is_open()) {
            file << bestTime;
            file.close();
        }
    }
}

int HighScore::getHighScore() {
    return bestTime;
}
