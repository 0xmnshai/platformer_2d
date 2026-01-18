#pragma once

#include "vector2f.hpp"

struct Sprite;
struct SpriteSheet;

class Tile
{
public:
    enum class TileType
    {
        // --- BASIC ---
        EMPTY, // No tile / air
        CLOUD,
        WALL,
        GROUND,
        SURFACE,

        // // --- SOLID GROUND ---
        // DIRT,             // Solid dirt
        // GRASS_TOP,        // Grass with dirt below
        // GRASS_LEFT_EDGE,  // Grass slope / edge (left)
        // GRASS_RIGHT_EDGE, // Grass slope / edge (right)

        // // --- SLOPES ---
        // SLOPE_UP_LEFT,    // Bottom-right to top-left
        // SLOPE_UP_RIGHT,   // Bottom-left to top-right
        // SLOPE_DOWN_LEFT,  // Top-right to bottom-left
        // SLOPE_DOWN_RIGHT, // Top-left to bottom-right

        // // --- PLATFORMS ---
        // WOOD_PLATFORM,  // Thin wooden platform
        // STONE_PLATFORM, // Thin stone platform

        // // --- DECORATION (NON-COLLIDABLE) ---
        // GRASS_DECOR, // Small grass tufts
        // ROCK_DECOR,  // Small rocks
        // VINES,       // Hanging vines
        // CLOUD,       // Background cloud

        // // --- INTERACTIVE ---
        // CRATE,  // Breakable box
        // SPRING, // Jump booster
        // SPIKES, // Damage tile

        // // --- COLLECTIBLES ---
        // GEM_RED, // Red gem
        // GEM_GREEN,
        // GEM_BLUE,

        // // --- HAZARDS ---
        // PIT,   // Kill zone
        // WATER, // Water / slow zone

        // // --- SPECIAL ---
        // CHECKPOINT, // Respawn point
        // GOAL,       // Level exit

        COUNT // Number of tile types (keep last)
    };

    Tile();
    Tile(int gridX, int gridY, TileType type = TileType::EMPTY, SpriteSheet *spriteSheet = nullptr);
    ~Tile() = default;

    TileType getType() const { return _type; }
    Vector2f getWorldPosition() const;
    Vector2f getCenter() const;
    int getGridX() const { return _gridX; }
    int getGridY() const { return _gridY; }

    SpriteSheet *getSpriteSheet() const { return _spriteSheet; }
    bool isSolid() const { return _type != TileType::EMPTY && _type != TileType::CLOUD; }
    Sprite getSprite() const;

    int getTileId() const { return _tileId; }
    void setTileId(int id) { _tileId = id; }

    void setSpriteSheet(SpriteSheet *sheet) { _spriteSheet = sheet; }
    void setType(TileType newType) { _type = newType; }

private:
    int _gridX;
    int _gridY;
    TileType _type;
    SpriteSheet *_spriteSheet;
    int _tileId;
};