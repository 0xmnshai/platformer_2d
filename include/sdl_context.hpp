#pragma once

class SDL_Context
{
public:
    SDL_Context();
    ~SDL_Context();

    SDL_Context(const SDL_Context &) = delete;
    SDL_Context &operator=(const SDL_Context &) = delete;

private:
    int _imgFlags;
};
