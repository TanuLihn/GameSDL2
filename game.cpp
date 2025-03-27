#include "game.h"
#include "win_level.h"

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 700;
const int GRID_SIZE = SCREEN_WIDTH / 7;  // Kích thước mỗi ô lưới

Game::Game() {
    window = nullptr;
    renderer = nullptr;
    isRunning = false;

    boxX = 0;         // Hộp bắt đầu từ cột đầu tiên
    boxY = 0;         // Ở hàng trên cùng
    boxSpeed = 10 * 3; // Tăng tốc độ nhanh hơn 1.5 lần
    isFalling = false;
    direction = true; // Di chuyển sang phải

    // Khởi tạo lưới (0: trống, 1: hộp đã rơi)
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            grid[i][j] = 0;
}

Game::~Game() {
    close();
}

bool Game::init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Xay Thung", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return false;
    }

    isRunning = true;
    return true;
}

void Game::run() {
    while (isRunning) {
        handleEvents();
        update();
        render();
        SDL_Delay(16);  // Khoảng 60 FPS
    }
}

void Game::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        if (event.type == SDL_KEYDOWN) {
            if (event.key.keysym.sym == SDLK_SPACE && !isFalling) {
                isFalling = true;  // Bắt đầu rơi khi nhấn Space
            }
        }
    }
}

void Game::update() {
    if (!isFalling) {
        // Di chuyển hộp qua lại
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
        // Xác định cột hiện tại của hộp
        int col = boxX / GRID_SIZE;

        // Tìm hàng trống thấp nhất trong cột
        int row = 6;
        while (row >= 0 && grid[row][col] == 1) {
            row--;
        }

        // Nếu tìm thấy vị trí hợp lệ, đặt hộp xuống
        if (row >= 0) {
            grid[row][col] = 1;
        }

        // Kiểm tra điều kiện thắng
        if (checkGameOver(grid)) {
            isRunning = false;
        }

        // Reset hộp mới
        boxX = 0;
        isFalling = false;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Vẽ lưới
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= 7; i++) {
        SDL_RenderDrawLine(renderer, i * GRID_SIZE, 0, i * GRID_SIZE, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE, SCREEN_WIDTH, i * GRID_SIZE);
    }

    // Vẽ hộp rơi
    SDL_Rect box = {boxX, boxY, GRID_SIZE, GRID_SIZE};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &box);

    // Vẽ hộp đã rơi xuống
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (grid[i][j] == 1) {
                SDL_Rect fallenBox = {j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE, GRID_SIZE};
                SDL_RenderFillRect(renderer, &fallenBox);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::close() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
