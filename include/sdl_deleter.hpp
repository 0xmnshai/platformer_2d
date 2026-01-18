#pragma once

#include <SDL2/SDL.h>

class SDL_Deleter
{
public:
    void operator()(SDL_Window *window) const { SDL_DestroyWindow(window); }
    void operator()(SDL_Renderer *renderer) const { SDL_DestroyRenderer(renderer); }
    void operator()(SDL_Texture *texture) const { SDL_DestroyTexture(texture); }
};