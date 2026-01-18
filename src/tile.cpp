#include "tile.hpp"
#include "config.hpp"

#include "spritesheet.hpp"

Tile::Tile()
    : _gridX(0), _gridY(0), _type(TileType::EMPTY), _spriteSheet(nullptr), _tileId(-1) {}

Tile::Tile(int gridX, int gridY, TileType type, SpriteSheet *spriteSheet)
    : _gridX(gridX), _gridY(gridY), _type(type), _spriteSheet(spriteSheet), _tileId(-1) {}

Sprite Tile::getSprite() const
{
    switch (_type)
    {
    case TileType::CLOUD:
        return SpriteSheet::TILE_CLOUD;
    case TileType::WALL:
        return SpriteSheet::TILE_WALL;
    case TileType::GROUND:
        return SpriteSheet::TILE_GROUND;
    case TileType::SURFACE:
        return SpriteSheet::TILE_SURFACE;
    default:
        return Sprite(0);  
    }

    return Sprite(0);
}

Vector2f Tile::getWorldPosition() const
{
    return Vector2f(_gridX * config::tile::size, _gridY * config::tile::size);
}

Vector2f Tile::getCenter() const
{
    return Vector2f(_gridX * config::tile::size + config::tile::size / 2.0f, _gridY * config::tile::size + config::tile::size / 2.0f);
}