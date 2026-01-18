
#include "input_handler.hpp"

void InputHandler::update()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
                running = false;
            break;
        default:
            break;
        }
    }
    keyState = SDL_GetKeyboardState(nullptr);
}

bool InputHandler::isKeyPressed(SDL_Keycode key) const
{
    return keyState && keyState[SDL_GetScancodeFromKey(key)];
}

bool InputHandler::isKeyDown(SDL_Scancode key) const
{
    return keyState && keyState[key];
}