#ifndef GAME_H
#define GAME_H
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h> // Thêm include cho SDL_mixer nếu bạn đang sử dụng nó
#include <string>
#include <vector>
#include "background.h"
#include "box.h"
#include "score.h"
#include "highscore.h"
#include "win_level.h"

class Button {
public:
    Button(int x, int y, int w, int h, const std::string& text);
    bool isMouseOver(int mouseX, int mouseY) const;
    void render(SDL_Renderer* renderer, TTF_Font* font);
    std::string getText() const { return text; }
private:
    SDL_Rect rect;
    std::string text;
    bool hover;
};

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
    enum GameState {
        MENU,
        DIFFICULTY_SELECT,
        INSTRUCTIONS,
        PLAYING,
        GAME_OVER,
        PAUSED
    };
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
    Score score;
    HighScore highScore;
    GameState gameState;
    Uint32 gameTime;
    Uint32 startTime;
    Uint32 pauseStartTime;
    Uint32 totalPauseTime;
    TTF_Font* font;
    TTF_Font* titleFont;
    Mix_Chunk* dropSound; // Thêm biến này để khắc phục lỗi
    std::vector<Button> menuButtons;
    std::vector<Button> difficultyButtons;
    void initButtons();
    void handleMenuEvents(SDL_Event& event);
    void handleDifficultySelectEvents(SDL_Event& event);
    void renderMenu();
    void renderDifficultySelect();
    void renderInstructions();
    void renderGame();
    void renderGameOver();
    void renderPaused();
    void renderText(const std::string& text, int x, int y, SDL_Color color, TTF_Font* customFont = nullptr);
    void renderTextAlternative(const std::string& text, int x, int y, SDL_Color color);
    void startGame(int difficulty);
    void resetGame();
    std::string formatTime(Uint32 timeInMs) const;
};

#endif
