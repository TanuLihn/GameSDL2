#include "background.h"
#include <iostream>

Background::Background(SDL_Renderer* renderer) {
    SDL_Surface* surface = IMG_Load("background.png");
    if (!surface) {
        std::cerr << "Failed to load background image: " << IMG_GetError() << std::endl;
        texture = nullptr;
        return;
    }
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
}

Background::~Background() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void Background::render(SDL_Renderer* renderer) {
    if (texture) {
        SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    }
}
