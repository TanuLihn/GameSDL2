#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SDL.h>
#include <SDL_image.h>

class Background {
public:
    Background(SDL_Renderer* renderer);
    ~Background();
    void render(SDL_Renderer* renderer);

private:
    SDL_Texture* texture;
};

#endif
