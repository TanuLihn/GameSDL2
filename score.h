#ifndef SCORE_H
#define SCORE_H

class Score {
private:
    int score;
public:
    Score();
    void addPoints(int points);
    int getScore() const;
    void reset();
};

#endif
