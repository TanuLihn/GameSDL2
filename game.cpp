#include "game.h"
#include <iostream>
#include <sstream>
#include <iomanip>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 700;
const int GRID_SIZE = SCREEN_WIDTH / 7;  // Kích thước mỗi ô lưới

// Button class implementation
Button::Button(int x, int y, int w, int h, const std::string& text)
    : rect{x, y, w, h}, text(text), hover(false) {}

bool Button::isMouseOver(int mouseX, int mouseY) const {
    return (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
            mouseY >= rect.y && mouseY <= rect.y + rect.h);
}

void Button::render(SDL_Renderer* renderer, TTF_Font* font) {
    // Draw button background
    if (hover) {
        SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255); // Highlighted color
    } else {
        SDL_SetRenderDrawColor(renderer, 70, 70, 200, 255); // Normal color
    }
    SDL_RenderFillRect(renderer, &rect);

    // Draw button border
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // Render text if font is available
    if (font) {
        SDL_Color textColor = {255, 255, 255, 255};
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(font, text.c_str(), textColor);
        if (textSurface) {
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
            if (texture) {
                SDL_Rect textRect = {
                    rect.x + (rect.w - textSurface->w) / 2,
                    rect.y + (rect.h - textSurface->h) / 2,
                    textSurface->w,
                    textSurface->h
                };
                SDL_RenderCopy(renderer, texture, NULL, &textRect);
                SDL_DestroyTexture(texture);
            }
            SDL_FreeSurface(textSurface);
        }
    }
}

// Game class implementation
Game::Game() {
    window = nullptr;
    renderer = nullptr;
    isRunning = false;

    boxX = 0;
    boxY = 0;
    boxSpeed = 2 * 10;
    isFalling = false;
    direction = true;

    gameState = MENU;
    gameTime = 0;
    startTime = 0;
    pauseStartTime = 0;
    totalPauseTime = 0;
    font = nullptr;
    titleFont = nullptr;

    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            grid[i][j] = 0;

    // Khởi tạo điểm số với 1000 điểm
    score.reset(1000);
}

Game::~Game() {
    close();
}

void Game::initButtons() {
    // Menu buttons
    menuButtons.clear();
    menuButtons.push_back(Button(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 200, 50, "Chơi"));
    menuButtons.push_back(Button(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 70, 200, 50, "Hướng dẫn"));

    // Difficulty buttons
    difficultyButtons.clear();
    difficultyButtons.push_back(Button(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 - 80, 200, 50, "Dễ"));
    difficultyButtons.push_back(Button(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2, 200, 50, "Trung Bình"));
    difficultyButtons.push_back(Button(SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 80, 200, 50, "Khó"));
    difficultyButtons.push_back(Button(50, SCREEN_HEIGHT - 80, 150, 50, "Quay lại"));
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Build The Box", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    // Thử tải nhiều font khác nhau và nhiều đường dẫn khác nhau
    const char* fontPaths[] = {
        "font.ttf",
        "assets/font.ttf",
        "fonts/font.ttf",
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/times.ttf",
        "C:/Windows/Fonts/tahoma.ttf",
        "C:/Windows/Fonts/segoeui.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "./data/font.ttf"
    };

    for (const char* path : fontPaths) {
        font = TTF_OpenFont(path, 24);
        if (font) {
            std::cout << "Successfully loaded font: " << path << std::endl;
            // Also load title font with larger size
            titleFont = TTF_OpenFont(path, 48);
            break;
        }
    }

    if (!font) {
        std::cerr << "Failed to load any font! TTF_Error: " << TTF_GetError() << std::endl;
        std::cerr << "Will use alternate rendering method for text" << std::endl;
        // Tiếp tục mà không có font - sẽ sử dụng phương pháp thay thế
    }

    background = new Background(renderer);
    box = new Box(renderer);

    if (!background->loadBackground("background.png") || !box->loadBoxTexture("box.png")) {
        return false;
    }

    initButtons();
    isRunning = true;
    return true;
}

void Game::renderText(const std::string& text, int x, int y, SDL_Color color, TTF_Font* customFont) {
    TTF_Font* fontToUse = customFont ? customFont : font;

    if (fontToUse) {
        // Render text sử dụng font nếu có
        SDL_Surface* textSurface = TTF_RenderUTF8_Solid(fontToUse, text.c_str(), color);
        if (!textSurface) {
            std::cerr << "Unable to render text surface! TTF_Error: " << TTF_GetError() << std::endl;
            renderTextAlternative(text, x, y, color);
            return;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if (!texture) {
            std::cerr << "Unable to create texture from rendered text! SDL_Error: " << SDL_GetError() << std::endl;
            renderTextAlternative(text, x, y, color);
        } else {
            SDL_Rect renderQuad = {x, y, textSurface->w, textSurface->h};
            SDL_RenderCopy(renderer, texture, NULL, &renderQuad);
            SDL_DestroyTexture(texture);
        }

        SDL_FreeSurface(textSurface);
    } else {
        // Sử dụng phương pháp thay thế nếu không có font
        renderTextAlternative(text, x, y, color);
    }
}

void Game::renderTextAlternative(const std::string& text, int x, int y, SDL_Color color) {
    int charWidth = 10;
    int charHeight = 20;
    int textWidth = static_cast<int>(text.length() * charWidth);

    // Tạo hình chữ nhật nền với viền
    SDL_Rect backgroundRect = {x - 5, y - 5, textWidth + 10, charHeight + 10};

    // Lưu màu hiện tại
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    // Vẽ nền
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200); // Nền tối mờ
    SDL_RenderFillRect(renderer, &backgroundRect);

    // Vẽ viền
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Viền trắng
    SDL_RenderDrawRect(renderer, &backgroundRect);

    // Vẽ text (dưới dạng hình chữ nhật nhỏ)
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Vẽ các "ký tự" giả như các dấu gạch ngang
    SDL_Rect charRect = {x, y, charWidth - 2, charHeight - 10};
    for (size_t i = 0; i < text.length(); i++) {
        charRect.x = x + i * charWidth;
        SDL_RenderFillRect(renderer, &charRect);
    }

    // Khôi phục màu vẽ trước đó
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void Game::run() {
    while (isRunning) {
        handleEvents();

        if (gameState == PLAYING) {
            update();
        }

        render();
        SDL_Delay(16);  // Khoảng 60fps
    }
}

void Game::handleMenuEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

        for (auto& button : menuButtons) {
            button.isMouseOver(mouseX, mouseY) ? true : false;
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        for (size_t i = 0; i < menuButtons.size(); i++) {
            if (menuButtons[i].isMouseOver(mouseX, mouseY)) {
                if (menuButtons[i].getText() == "Chơi") {
                    gameState = DIFFICULTY_SELECT;
                } else if (menuButtons[i].getText() == "Hướng dẫn") {
                    gameState = INSTRUCTIONS;
                }
            }
        }
    }
}

void Game::handleDifficultySelectEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEMOTION) {
        int mouseX = event.motion.x;
        int mouseY = event.motion.y;

        for (auto& button : difficultyButtons) {
            button.isMouseOver(mouseX, mouseY) ? true : false;
        }
    }

    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        for (size_t i = 0; i < difficultyButtons.size(); i++) {
            if (difficultyButtons[i].isMouseOver(mouseX, mouseY)) {
                if (difficultyButtons[i].getText() == "Dễ") {
                    startGame(2); // Easy speed multiplier
                } else if (difficultyButtons[i].getText() == "Trung Bình") {
                    startGame(3); // Medium speed multiplier
                } else if (difficultyButtons[i].getText() == "Khó") {
                    startGame(5); // Hard speed multiplier
                } else if (difficultyButtons[i].getText() == "Quay lại") {
                    gameState = MENU;
                }
            }
        }
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }

        // Handle key presses globally
        if (event.type == SDL_KEYDOWN) {
            switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    if (gameState == PLAYING || gameState == PAUSED) {
                        gameState = MENU;
                    } else if (gameState == DIFFICULTY_SELECT || gameState == INSTRUCTIONS) {
                        gameState = MENU;
                    } else if (gameState == MENU) {
                        isRunning = false;
                    }
                    break;
            }
        }

        // Handle state-specific events
        switch (gameState) {
            case MENU:
                handleMenuEvents(event);
                break;

            case DIFFICULTY_SELECT:
                handleDifficultySelectEvents(event);
                break;

            case INSTRUCTIONS:
                if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_RETURN) {
                    gameState = MENU;
                }
                break;

            case PLAYING:
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                            if (!isFalling) {
                                isFalling = true;
                            }
                            break;
                        case SDLK_p:
                            gameState = PAUSED;
                            pauseStartTime = SDL_GetTicks();
                            break;
                        case SDLK_r:
                            resetGame();
                            break;
                    }
                }
                break;

            case PAUSED:
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_p:
                            gameState = PLAYING;
                            totalPauseTime += SDL_GetTicks() - pauseStartTime;
                            break;
                    }
                }
                break;

            case GAME_OVER:
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                            resetGame();
                            gameState = MENU;
                            break;
                    }
                }
                break;
        }
    }
}

void Game::update() {
    // Calculate current game time (subtracting pause time)
    if (gameState == PLAYING) {
        gameTime = SDL_GetTicks() - startTime - totalPauseTime;
    }

    if (!isFalling) {
        if (direction) {
            boxX += boxSpeed;
            if (boxX + GRID_SIZE >= SCREEN_WIDTH) {
                direction = false;
            }
        } else {
            boxX -= boxSpeed;
            if (boxX <= 0) {
                direction = true;
            }
        }
    } else {
        int col = boxX / GRID_SIZE;
        if (col >= 0 && col < 7) {
            int row = 6;
            while (row >= 0 && grid[row][col] == 1) {
                row--;
            }

            if (row >= 0) {
                grid[row][col] = 1;
                score.subtractPoints(10); // Trừ 10 điểm khi một hộp được đặt
            }

            // Kiểm tra điều kiện kết thúc game
            if (checkGameOver(grid)) {
                gameState = GAME_OVER;
                highScore.saveHighScore(gameTime);
            }
        }

        boxX = 0;
        isFalling = false;
    }
}

std::string Game::formatTime(Uint32 timeInMs) const {
    Uint32 totalSeconds = timeInMs / 1000;
    Uint32 milliseconds = timeInMs % 1000;
    Uint32 seconds = totalSeconds % 60;
    Uint32 minutes = (totalSeconds / 60) % 60;

    std::stringstream formatted;
    formatted << std::setfill('0') << std::setw(2) << minutes << ":"
              << std::setfill('0') << std::setw(2) << seconds << "."
              << std::setfill('0') << std::setw(3) << milliseconds;

    return formatted.str();
}

void Game::renderMenu() {
    // Draw background
    background->render();

    // Draw title
    SDL_Color titleColor = {255, 255, 0, 255}; // Yellow
    if (titleFont) {
        renderText("Build The Box", SCREEN_WIDTH/2 - 150, SCREEN_HEIGHT/4, titleColor, titleFont);
    } else {
        renderText("Build The Box", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/4, titleColor);
    }

    // Draw buttons
    for (auto& button : menuButtons) {
        button.render(renderer, font);
    }
}

void Game::renderDifficultySelect() {
    // Draw background
    background->render();

    // Draw title
    SDL_Color titleColor = {255, 255, 0, 255}; // Yellow
    renderText("Chọn độ khó", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/4, titleColor);

    // Draw buttons
    for (auto& button : difficultyButtons) {
        button.render(renderer, font);
    }
}

void Game::renderInstructions() {
    // Draw background
    background->render();

    // Draw title
    SDL_Color titleColor = {255, 255, 0, 255}; // Yellow
    renderText("Hướng dẫn", SCREEN_WIDTH/2 - 80, 50, titleColor);

    // Draw instructions
    SDL_Color textColor = {255, 255, 255, 255}; // White
    std::vector<std::string> instructions = {
        "Nhiệm vụ của bạn là cố gắng sắp xếp các hộp",
        "sao cho có 1 cột đạt 7 hộp hoặc 1 hàng đạt",
        "7 hộp nhanh nhất có thể mà sao cho điểm",
        "của bạn cũng phải cao nhất.",
        "",
        "Ban đầu bạn sẽ có 1000 điểm.",
        "Mỗi lần hộp rơi điểm sẽ bị trừ 10.",
        "",
        "Điều khiển:",
        "SPACE: Thả hộp",
        "P: Tạm dừng",
        "R: Chơi lại",
        "ESC: Quay lại menu"
    };

    for (size_t i = 0; i < instructions.size(); i++) {
        renderText(instructions[i], 50, 120 + i * 35, textColor);
    }

    renderText("Nhấn ENTER để quay lại", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT - 80, titleColor);
}

void Game::renderGame() {
    // Vẽ lưới
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= 7; i++) {
        SDL_RenderDrawLine(renderer, i * GRID_SIZE, 0, i * GRID_SIZE, SCREEN_WIDTH);
        SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE, SCREEN_WIDTH, i * GRID_SIZE);
    }

    // Vẽ các ô đã xếp
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (grid[i][j] == 1) {
                box->render(j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE);
            }
        }
    }

    // Vẽ ô đang di chuyển
    box->render(boxX, boxY, GRID_SIZE);

    // Hiển thị thông tin game với màu dễ nhìn hơn
    SDL_Color textColor = {255, 255, 0, 255}; // Màu vàng cho text

    // Hiển thị điểm ở góc dưới bên phải
    std::stringstream scoreText;
    scoreText << "Điểm: " << score.getScore();
    int scoreWidth = 150; // Giá trị mặc định nếu không có font
    int scoreHeight = 30;
    if (font) {
        TTF_SizeUTF8(font, scoreText.str().c_str(), &scoreWidth, &scoreHeight);
    }
    renderText(scoreText.str(), SCREEN_WIDTH - scoreWidth - 20, SCREEN_HEIGHT - scoreHeight - 20, textColor);

    // Hiển thị đồng hồ bấm giờ ở góc dưới bên trái
    renderText("Thời gian: " + formatTime(gameTime), 20, SCREEN_HEIGHT - scoreHeight - 20, textColor);

    // Hiển thị high score ở phía trên
    std::stringstream highScoreText;
    highScoreText << "Thời gian tốt nhất: ";
    if (highScore.getHighScore() != -1) {
        highScoreText << formatTime(highScore.getHighScore());
    } else {
        highScoreText << "Chưa có";
    }
    renderText(highScoreText.str(), 10, SCREEN_WIDTH + 20, textColor);
}

void Game::renderGameOver() {
    // Vẽ một hình chữ nhật bán trong suốt làm nền cho thông báo
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_Rect overlay = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 3};
    SDL_RenderFillRect(renderer, &overlay);

    // Vẽ viền cho hình chữ nhật
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &overlay);

    // Sử dụng màu đỏ cho thông báo kết thúc
    SDL_Color gameOverColor = {255, 0, 0, 255};
    renderText("KẾT THÚC!", SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 - 80, gameOverColor);

    // Hiển thị thời gian và điểm số
    SDL_Color textColor = {255, 255, 0, 255};

    std::stringstream finalTimeText;
    finalTimeText << "Thời gian: " << formatTime(gameTime);
    renderText(finalTimeText.str(), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 30, textColor);

    std::stringstream finalScoreText;
    finalScoreText << "Điểm cuối: " << score.getScore();
    renderText(finalScoreText.str(), SCREEN_WIDTH / 2 - 80, SCREEN_HEIGHT / 2 + 10, textColor);

    // Sử dụng màu trắng cho hướng dẫn
    SDL_Color instructionColor = {255, 255, 255, 255};
    renderText("Nhấn SPACE để quay lại menu", SCREEN_WIDTH / 2 - 120, SCREEN_HEIGHT / 2 + 50, instructionColor);
}

void Game::renderPaused() {
    // Vẽ một hình chữ nhật bán trong suốt làm nền cho thông báo tạm dừng
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 150);
    SDL_Rect overlay = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 3, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 6};
    SDL_RenderFillRect(renderer, &overlay);

    // Vẽ viền
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &overlay);

    // Sử dụng màu xanh cho thông báo tạm dừng
    SDL_Color pauseColor = {0, 255, 255, 255};
    renderText("TẠM DỪNG", SCREEN_WIDTH / 2 - 60, SCREEN_HEIGHT / 2 - 30, pauseColor);

    SDL_Color textColor = {255, 255, 0, 255};
    renderText("Nhấn P để tiếp tục", SCREEN_WIDTH / 2 - 90, SCREEN_HEIGHT / 2, textColor);
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    background->render();

    switch (gameState) {
        case MENU:
            renderMenu();
            break;

        case DIFFICULTY_SELECT:
            renderDifficultySelect();
            break;

        case INSTRUCTIONS:
            renderInstructions();
            break;

        case PLAYING:
            renderGame();
            break;

        case PAUSED:
            renderGame();
            renderPaused();
            break;

        case GAME_OVER:
            renderGame();
            renderGameOver();
            break;
    }

    SDL_RenderPresent(renderer);
}

void Game::startGame(int difficulty) {
    resetGame();
    boxSpeed = difficulty * 10;
    gameState = PLAYING;
    startTime = SDL_GetTicks();
    totalPauseTime = 0;
}

void Game::resetGame() {
    // Làm trống lưới
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            grid[i][j] = 0;

    boxX = 0;
    boxY = 0;
    isFalling = false;
    direction = true;

    score.reset(1000); // Đặt lại điểm số thành 1000
    gameTime = 0;
}

void Game::close() {
    if (font) {
        TTF_CloseFont(font);
    }

    if (titleFont) {
        TTF_CloseFont(titleFont);
    }

    delete background;
    delete box;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

}
