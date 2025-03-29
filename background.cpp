#include "background.h"
#include <iostream>

Background::Background(SDL_Renderer* renderer) {
    this->renderer = renderer;
    backgroundTexture = nullptr;
}

Background::~Background() {
    SDL_DestroyTexture(backgroundTexture);
}

bool Background::loadBackground(const std::string& path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cerr << "Failed to load background image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    backgroundTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (!backgroundTexture) {
        std::cerr << "Failed to create texture from background image! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Background::render() {
    if (backgroundTexture) {
        SDL_RenderCopy(renderer, backgroundTexture, NULL, NULL);
    }
}
