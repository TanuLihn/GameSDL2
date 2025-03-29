#ifndef HIGHSCORE_H
#define HIGHSCORE_H

class HighScore {
private:
    int bestTime;
public:
    HighScore();
    void loadHighScore();
    void saveHighScore(int time);
    int getHighScore();
};

#endif
