#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>

class HighScore {
private:
    std::vector<std::pair<std::string, int>> scores;
public:
    HighScore();
    void addScore(const std::string& name, int score);
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);
    void display() const;
};

#endif
