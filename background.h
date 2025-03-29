#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Background {
public:
    Background(SDL_Renderer* renderer);
    ~Background();

    bool loadBackground(const std::string& path);
    void render();

private:
    SDL_Renderer* renderer;
    SDL_Texture* backgroundTexture;
};

#endif
