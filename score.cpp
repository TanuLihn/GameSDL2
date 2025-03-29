#include "score.h"

Score::Score() : score(0) {}

void Score::addPoints(int points) {
    score += points;
}

void Score::subtractPoints(int points) {
    score -= points;
    if (score < 0) score = 0; // Đảm bảo điểm số không âm
}

int Score::getScore() const {
    return score;
}

void Score::reset(int initialScore) {
    score = initialScore;
}
