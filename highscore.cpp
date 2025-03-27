#include "highscore.h"

HighScore::HighScore() {}

void HighScore::addScore(const std::string& name, int score) {
    scores.push_back({name, score});
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
}

void HighScore::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for writing!" << std::endl;
        return;
    }
    for (const auto& entry : scores) {
        file << entry.first << " " << entry.second << "\n";
    }
    file.close();
}

void HighScore::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for reading!" << std::endl;
        return;
    }
    scores.clear();
    std::string name;
    int score;
    while (file >> name >> score) {
        scores.push_back({name, score});
    }
    file.close();
    std::sort(scores.begin(), scores.end(), [](const auto& a, const auto& b) {
        return a.second > b.second;
    });
}

void HighScore::display() const {
    std::cout << "High Scores:" << std::endl;
    for (const auto& entry : scores) {
        std::cout << entry.first << ": " << entry.second << std::endl;
    }
}
