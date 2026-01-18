
#include <string>
#include <stdexcept>

#include <SDL2/SDL.h> 
#include <SDL2/SDL_image.h>

#include "sdl_context.hpp"

SDL_Context::SDL_Context()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        throw std::runtime_error(std::string("Failed to initialize SDL: ") + SDL_GetError());
    }

    _imgFlags = IMG_INIT_PNG;
    if ((IMG_Init(_imgFlags) & _imgFlags) != _imgFlags)
    {
        SDL_Quit();
        throw std::runtime_error(std::string("Failed to initialize SDL_image: ") + IMG_GetError());
    }
}

SDL_Context::~SDL_Context()
{
    IMG_Quit();
    SDL_Quit();
}