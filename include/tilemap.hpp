#pragma once

#include <SDL2/SDL.h>

#include "config.hpp"
#include "vector2f.hpp"
#include "tile.hpp"
#include "spritesheet.hpp"
#include "camera.hpp"

class TileMap
{
public:
    TileMap(SpriteSheet *spriteSheet, int width, int height);
    ~TileMap() = default;

    void setTileScreen(int sx, int sy, Tile::TileType type);

    void setTile(int gridX, int gridY, Tile::TileType type);
    Tile *getTile(int gridX, int gridY);
    const Tile *getTile(int gridX, int gridY) const;
    bool isTileSolid(int gridX, int gridY) const;

    int getMapWidth() const { return _mapWidth; }
    int getMapHeight() const { return _mapHeight; }
    int getTileSize() const { return config::tile::size; }

    void render(SDL_Renderer *renderer) const;
    void renderWithCamera(SDL_Renderer *renderer, const Camera &camera) const;
    void clear();
    void fillRectangle(int startX, int startY, int endX, int endY, Tile::TileType type);
    void loadFromJSON(const std::string &filePath);

private:
    int _mapWidth;
    int _mapHeight;
    SpriteSheet *_spriteSheet;
    std::vector<std::vector<Tile>> _tiles;
};