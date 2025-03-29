#include "box.h"
#include <iostream>

Box::Box(SDL_Renderer* renderer) {
    this->renderer = renderer;
    boxTexture = nullptr;
}

Box::~Box() {
    SDL_DestroyTexture(boxTexture);
}

bool Box::loadBoxTexture(const std::string& path) {
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (!loadedSurface) {
        std::cerr << "Failed to load box image! SDL_image Error: " << IMG_GetError() << std::endl;
        return false;
    }

    boxTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);

    if (!boxTexture) {
        std::cerr << "Failed to create texture from box image! SDL Error: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Box::render(int x, int y, int size) {
    if (boxTexture) {
        SDL_Rect renderQuad = {x, y, size, size};
        SDL_RenderCopy(renderer, boxTexture, NULL, &renderQuad);
    }
}
