#include "score.h"

Score::Score() : score(0) {}

void Score::addPoints(int points) {
    score += points;
}

int Score::getScore() const {
    return score;
}

void Score::reset() {
    score = 0;
}
