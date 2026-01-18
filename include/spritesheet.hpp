#pragma once

#include <string>
#include <memory> 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "sprite.hpp"
#include "sdl_deleter.hpp"

class SpriteSheet
{
public:
    SpriteSheet(SDL_Renderer *renderer, const std::string &filePath);
    ~SpriteSheet() = default;

    void renderSprite(const Sprite &sprite, int screenX, int screenY);
    SDL_Texture *getTexture() const { return _texture.get(); }

    static Sprite PLAYER_IDLE_LEFT;
    static Sprite PLAYER_IDLE_RIGHT;
    static Sprite PLAYER_JUMP_LEFT;
    static Sprite PLAYER_JUMP_RIGHT;

    static Sprite TILE_CLOUD;
    static Sprite TILE_WALL;
    static Sprite TILE_GROUND;
    static Sprite TILE_SURFACE;

private:
    std::unique_ptr<SDL_Texture, SDL_Deleter> _texture;
    SDL_Renderer *_renderer;
};
