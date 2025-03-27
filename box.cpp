#include "box.h"
#include <SDL_image.h>

Box::Box(int x, int y, int size, SDL_Renderer* renderer)
    : x(x), y(y), size(size), renderer(renderer), falling(false), velocityY(0) {
    // Load texture
    SDL_Surface* surface = IMG_Load("box.png");
    if (surface) {
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    } else {
        texture = nullptr;
    }
}

Box::~Box() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Box::update() {
    if (falling) {
        y += velocityY;  // Cập nhật vị trí rơi xuống
        velocityY += 1;  // Tăng tốc độ rơi (giống trọng lực)
    }
}

void Box::render() {
    if (texture) {
        SDL_Rect boxRect = {x, y, size, size};
        SDL_RenderCopy(renderer, texture, nullptr, &boxRect);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ
        SDL_Rect boxRect = {x, y, size, size};
        SDL_RenderFillRect(renderer, &boxRect);
    }
}

void Box::drop() {
    falling = true;
    velocityY = 5;  // Bắt đầu rơi với vận tốc ban đầu
}
