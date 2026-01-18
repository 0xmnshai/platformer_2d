#pragma once

#include <SDL2/SDL.h>

class InputHandler
{
public:
    void update();
    bool isRunning() const { return running; }
    bool isKeyPressed(SDL_Keycode key) const;
    bool isKeyDown(SDL_Scancode key) const;

private:
    bool running = true;
    const Uint8 *keyState = nullptr;
};
