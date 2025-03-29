#ifndef BOX_H
#define BOX_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class Box {
public:
    Box(SDL_Renderer* renderer);
    ~Box();

    bool loadBoxTexture(const std::string& path);
    void render(int x, int y, int size);

private:
    SDL_Renderer* renderer;
    SDL_Texture* boxTexture;
};

#endif
