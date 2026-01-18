
#include <iostream>

#include "spritesheet.hpp"

// Sprite -> COLUMN , ROW
Sprite SpriteSheet::TILE_CLOUD(23);
Sprite SpriteSheet::TILE_WALL(19);
Sprite SpriteSheet::TILE_GROUND(3);
Sprite SpriteSheet::TILE_SURFACE(4);

// using that enemy as player for now : col : 3 , row , 6
Sprite SpriteSheet::PLAYER_IDLE_LEFT(0);

SpriteSheet::SpriteSheet(SDL_Renderer *renderer, const std::string &filePath)
    : _renderer(renderer)
{
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if (!surface)
    {
        throw std::runtime_error(std::string("Failed to load spritesheet: ") + IMG_GetError());
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    if (!texture)
    {
        throw std::runtime_error(std::string("Failed to create spritesheet texture: ") + SDL_GetError());
    }

    _texture.reset(texture);
}

void SpriteSheet::renderSprite(const Sprite &sprite, int screenX, int screenY)
{
    if (!_texture || !_renderer)
        throw std::runtime_error("Renderer or texture not initialized");

    const int TILE_SIZE = config::tile::size;
    int scaledTileSize = TILE_SIZE * config::tile::scale;

    int src_x = (sprite.index % config::tile::tiles_per_row) * config::tile::size;
    int src_y = (sprite.index / config::tile::tiles_per_row) * config::tile::size;

    SDL_Rect srcRect = {src_x, src_y, config::tile::size, config::tile::size};

    SDL_Rect dstRect{
        screenX,
        screenY,
        scaledTileSize,
        scaledTileSize};

    SDL_RenderCopyEx(
        _renderer,
        _texture.get(),
        &srcRect,
        &dstRect,
        0,
        nullptr,
        SDL_FLIP_NONE);
}
