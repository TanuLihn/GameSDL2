#include "game.h"
#include <iostream>

const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 700;
const int GRID_SIZE = SCREEN_WIDTH / 7;  // Kích thước mỗi ô lưới

Game::Game() {
    window = nullptr;
    renderer = nullptr;
    isRunning = false;

    boxX = 0;
    boxY = 0;
    boxSpeed = 4 * 10;
    isFalling = false;
    direction = true;

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

    background = new Background(renderer);
    box = new Box(renderer);

    if (!background->loadBackground("background.png") || !box->loadBoxTexture("box.png")) {
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
        SDL_Delay(16);
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
                isFalling = true;
            }
        }
    }
}

void Game::update() {
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
        int row = 6;
        while (row >= 0 && grid[row][col] == 1) {
            row--;
        }

        if (row >= 0) {
            grid[row][col] = 1;
        }

        boxX = 0;
        isFalling = false;
    }
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    background->render();

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    for (int i = 0; i <= 7; i++) {
        SDL_RenderDrawLine(renderer, i * GRID_SIZE, 0, i * GRID_SIZE, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * GRID_SIZE, SCREEN_WIDTH, i * GRID_SIZE);
    }

    box->render(boxX, boxY, GRID_SIZE);

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if (grid[i][j] == 1) {
                box->render(j * GRID_SIZE, i * GRID_SIZE, GRID_SIZE);
            }
        }
    }

    SDL_RenderPresent(renderer);
}

void Game::close() {
    delete background;
    delete box;
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
