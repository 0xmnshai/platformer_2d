
#include <math.h>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include "tilemap.hpp"
#include "config.hpp"

TileMap::TileMap(SpriteSheet *spriteSheet, int width, int height)
    : _mapWidth(width), _mapHeight(height), _spriteSheet(spriteSheet)
{
    _tiles.resize(config::tile::screen_tiles_y);
    for (int i = 0; i < config::tile::screen_tiles_y; ++i)
    {
        _tiles[i].resize(config::tile::screen_tiles_x);
        for (int j = 0; j < config::tile::screen_tiles_x; ++j)
        {
            _tiles[i][j] = Tile(j, i, Tile::TileType::EMPTY, spriteSheet);
        }
    }
}

void TileMap::setTileScreen(int sx, int sy, Tile::TileType type)
{
    if (sx < 0 || sx >= config::tile::screen_tiles_x ||
        sy < 0 || sy >= config::tile::screen_tiles_y)
        return;

    _tiles[sy][sx].setType(type);
}

void TileMap::setTile(int gridX, int gridY, Tile::TileType type)
{
    if (gridX >= 0 && gridX < _mapWidth && gridY >= 0 && gridY < _mapHeight)
    {
        _tiles[gridY][gridX].setType(type);
    }
}

Tile *TileMap::getTile(int gridX, int gridY)
{
    if (gridX >= 0 && gridX < _mapWidth && gridY >= 0 && gridY < _mapHeight)
    {
        return &_tiles[gridY][gridX];
    }
    return nullptr;
}

const Tile *TileMap::getTile(int gridX, int gridY) const
{
    if (gridX >= 0 && gridX < _mapWidth && gridY >= 0 && gridY < _mapHeight)
    {
        return &_tiles[gridY][gridX];
    }
    return nullptr;
}

bool TileMap::isTileSolid(int gridX, int gridY) const
{
    const Tile *tile = getTile(gridX, gridY);
    return tile && tile->isSolid();
}

void TileMap::fillRectangle(int startX, int startY, int endX, int endY, Tile::TileType type)
{
    for (int y = startY; y <= endY; ++y)
    {
        for (int x = startX; x <= endX; ++x)
        {
            setTile(x, y, type);
        }
    }
}

void TileMap::render(SDL_Renderer *renderer) const
{
    const int TILE_SIZE = config::tile::size;
    int scaledTileSize = TILE_SIZE * config::tile::scale;

    for (int y = 0; y < _mapHeight; ++y)
    {
        for (int x = 0; x < _mapWidth; ++x)
        {
            const Tile &tile = _tiles[y][x];
            if (tile.getType() != Tile::TileType::EMPTY)
            {
                Vector2f worldPos = tile.getWorldPosition();

                Sprite sprite = tile.getSprite();

                int src_x = (sprite.index % config::tile::tiles_per_row) * config::tile::size;
                int src_y = (sprite.index / config::tile::tiles_per_row) * config::tile::size;

                SDL_Rect srcRect = {src_x, src_y, config::tile::size, config::tile::size};

                SDL_Rect dstRect = {static_cast<int>(worldPos.x), static_cast<int>(worldPos.y),
                                    scaledTileSize, scaledTileSize};

                if (_spriteSheet && _spriteSheet->getTexture())
                {
                    SDL_RenderCopy(renderer, _spriteSheet->getTexture(), &srcRect, &dstRect);
                }
            }
        }
    }
}

// void TileMap::renderWithCamera(SDL_Renderer *renderer, const Camera &camera) const
// {
//     Vector2f cameraPos = camera.getPosition();

//     const int TILE_SIZE = config::tile::size;
//     int scaledTileSize = TILE_SIZE * config::tile::scale;

//     int viewportWidth = camera.getViewportWidth();
//     int viewportHeight = camera.getViewportHeight();

//     int startTileX = std::max(0, static_cast<int>(cameraPos.x / TILE_SIZE) - 1);
//     int endTileX = std::min(_mapWidth - 1, static_cast<int>((cameraPos.x + viewportWidth) / TILE_SIZE) + 1);

//     int startTileY = std::max(0, static_cast<int>(cameraPos.y / TILE_SIZE) - 1);
//     int endTileY = std::min(_mapHeight - 1, static_cast<int>((cameraPos.y + viewportHeight) / TILE_SIZE) + 1);

//     for (int y = startTileY; y <= endTileY; ++y)
//     {
//         for (int x = startTileX; x <= endTileX; ++x)
//         {
//             const Tile &tile = _tiles[y][x];
//             if (tile.getType() != Tile::TileType::EMPTY)
//             {
//                 Vector2f worldPos = tile.getWorldPosition();
//                 Vector2f screenPos = camera.worldToScreen(worldPos);

//                 Sprite sprite = tile.getSprite();

//                 int src_x = (sprite.index % config::tile::tiles_per_row);
//                 int src_y = (sprite.index / config::tile::tiles_per_row);

//                 SDL_Rect srcRect = {src_x, src_y, config::tile::size, config::tile::size};
//                 SDL_Rect dstRect = {static_cast<int>(screenPos.x), static_cast<int>(screenPos.y),
//                                     scaledTileSize, scaledTileSize};

//                 if (_spriteSheet && _spriteSheet->getTexture())
//                 {
//                     SDL_RenderCopy(renderer, _spriteSheet->getTexture(), &srcRect, &dstRect);
//                 }
//             }
//         }
//     }
// }

void TileMap::renderWithCamera(SDL_Renderer *renderer, const Camera &camera) const
{
    const int tileSize = config::tile::scaled_size;

    for (int y = 0; y < config::tile::screen_tiles_y; ++y)
    {
        for (int x = 0; x < config::tile::screen_tiles_x; ++x)
        {
            const Tile &tile = _tiles[y][x];
            if (tile.getType() == Tile::TileType::EMPTY)
                continue;

            Vector2f screenPos{
                float(x * tileSize),
                float(y * tileSize)};

            Vector2f worldPos = screenPos + camera.getPosition();

            Sprite sprite = tile.getSprite();

            SDL_Rect src{
                (sprite.index % config::tile::tiles_per_row) * config::tile::size,
                (sprite.index / config::tile::tiles_per_row) * config::tile::size,
                config::tile::size,
                config::tile::size};

            SDL_Rect dst{
                int(worldPos.x - camera.getPosition().x),
                int(worldPos.y - camera.getPosition().y),
                tileSize,
                tileSize};

            SDL_RenderCopy(renderer, _spriteSheet->getTexture(), &src, &dst);
        }
    }
}

void TileMap::clear()
{
    for (int y = 0; y < config::tile::screen_tiles_y; ++y)
    {
        for (int x = 0; x < config::tile::screen_tiles_x; ++x)
        {
            _tiles[y][x].setType(Tile::TileType::EMPTY);
        }
    }
}

// void TileMap::loadFromJSON(const std::string &filePath)
// {
//     try
//     {
//         this->clear();

//         std::ifstream file(filePath);

//         if (!file.is_open())
//         {
//             throw std::runtime_error("Failed to open file: " + filePath);
//         };

//         json jsonData;
//         file >> jsonData;
//         file.close();

//         // _mapWidth = jsonData.value("mapWidth", _mapWidth);
//         // _mapHeight = jsonData.value("mapHeight", _mapHeight);

//         // _tiles.resize(_mapHeight);

//         for (int i = 0; i < _mapHeight; ++i)
//         {
//             _tiles[i].resize(_mapWidth);
//             for (int j = 0; j < _mapWidth; ++j)
//             {
//                 _tiles[i][j] = Tile(j, i, Tile::TileType::EMPTY, _spriteSheet);
//             }
//         }

//         if (jsonData.contains("layers") && jsonData["layers"].is_array())
//         {
//             for (const auto &layer : jsonData["layers"])
//             {
//                 if (!layer.contains("name") || !layer.contains("tiles"))
//                     continue;

//                 std::string layerName = layer["name"].get<std::string>();
//                 bool isCollider = layer.contains("collider") && layer["collider"].get<bool>();

//                 if (!layer["tiles"].is_array())
//                     continue;

//                 Tile::TileType tileType = Tile::TileType::EMPTY;

//                 if (layer["name"] == "Surface")
//                 {
//                     tileType = Tile::TileType::SURFACE;
//                 }
//                 else if (layer["name"] == "Wall")
//                 {
//                     tileType = Tile::TileType::WALL;
//                 }
//                 else if (layer["name"] == "Ground")
//                 {
//                     tileType = Tile::TileType::GROUND;
//                 }
//                 else if (layer["name"] == "Cloud")
//                 {
//                     tileType = Tile::TileType::CLOUD;
//                 }

//                 for (const auto &tile : layer["tiles"])
//                 {
//                     if (!tile.contains("x") || !tile.contains("y"))
//                         continue;

//                     int x = tile.at("x").get<int>();
//                     int y = tile.at("y").get<int>();

//                     if (x < 0 || x >= _mapWidth || y < 0 || y >= _mapHeight)
//                     {
//                         continue;
//                     }

//                     if (tileType != Tile::TileType::EMPTY)
//                     {
//                         setTile(x, y, tileType);
//                     }
//                 }
//             }
//         }
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error loading tilemap from JSON: " << e.what() << std::endl;
//     }
// }

// void TileMap::loadFromJSON(const std::string &filePath)
// {
//     try
//     {
//         clear();

//         std::ifstream file(filePath);
//         if (!file.is_open())
//         {
//             throw std::runtime_error("Failed to open file: " + filePath);
//         }

//         json jsonData;
//         file >> jsonData;

//         if (!jsonData.contains("layers") || !jsonData["layers"].is_array())
//             return;

//         for (const auto &layer : jsonData["layers"])
//         {
//             if (!layer.contains("name") || !layer.contains("tiles"))
//                 continue;

//             if (!layer["tiles"].is_array())
//                 continue;

//             Tile::TileType tileType = Tile::TileType::EMPTY;

//             const std::string name = layer["name"].get<std::string>();

//             if (name == "Surface")
//                 tileType = Tile::TileType::SURFACE;
//             else if (name == "Wall")
//                 tileType = Tile::TileType::WALL;
//             else if (name == "Ground")
//                 tileType = Tile::TileType::GROUND;
//             else if (name == "Cloud")
//                 tileType = Tile::TileType::CLOUD;

//             for (const auto &tile : layer["tiles"])
//             {
//                 if (!tile.contains("x") || !tile.contains("y"))
//                     continue;

//                 int sx = tile["x"].get<int>();
//                 int sy = tile["y"].get<int>();

//                 setTileScreen(sx, sy, tileType);
//             }
//         }
//     }
//     catch (const std::exception &e)
//     {
//         std::cerr << "Error loading tilemap from JSON: " << e.what() << std::endl;
//     }
// }

void TileMap::loadFromJSON(const std::string &filePath)
{
    try
    {
        clear();

        std::ifstream file(filePath);
        if (!file.is_open())
            throw std::runtime_error("Failed to open file: " + filePath);

        json jsonData;
        file >> jsonData;

        if (!jsonData.contains("layers"))
            return;

        for (const auto &layer : jsonData["layers"])
        {
            if (!layer.contains("name") || !layer.contains("tiles"))
                continue;

            Tile::TileType tileType = Tile::TileType::EMPTY;
            const std::string name = layer["name"];

            if (name == "Surface")
                tileType = Tile::TileType::SURFACE;
            else if (name == "Wall")
                tileType = Tile::TileType::WALL;
            else if (name == "Ground")
                tileType = Tile::TileType::GROUND;
            else if (name == "Cloud")
                tileType = Tile::TileType::CLOUD;

            for (const auto &tile : layer["tiles"])
            {
                int x = tile.value("x", -1);
                int y = tile.value("y", -1);

                setTileScreen(x, y, tileType);
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error loading tilemap: " << e.what() << std::endl;
    }
}
