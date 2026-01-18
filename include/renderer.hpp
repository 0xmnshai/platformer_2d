
#pragma once

#include <SDL2/SDL.h>
#include <string>

#include "sdl_deleter.hpp"
#include "spritesheet.hpp"
#include "entity.hpp"
#include "tilemap.hpp"

class
    Renderer
{
public:
    Renderer(const std::string &title, int width, int height);
    ~Renderer();

    void clear(Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, Uint8 a = 255);
    void present();
    void loadTexture(const std::string &filePath, std::unique_ptr<SDL_Texture, SDL_Deleter> &outTexture);
    void loadSpriteSheet(const std::string &filePath);
    void loadPlayerSpriteSheet(const std::string &filePath);
    void drawEntity(const Entity &entity);
    void drawTileMap(const TileMap &tileMap);
    void drawTileMapWithCamera(const TileMap &tileMap, const Camera &camera);
    void drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    // void drawMovingPlatform(const MovingPlatform &platform);
    void drawSprite(const Sprite &sprite, int x, int y, int w, int h);
    void drawPlayerSprite(const Sprite &sprite, int x, int y, int w, int h);

    float getRefreshRate() const;
    SDL_Renderer *getSDLRenderer() const { return _renderer.get(); }
    SpriteSheet *getSpriteSheet() const { return _spriteSheet.get(); }
    SpriteSheet *getPlayerSpriteSheet() const { return _playerSpriteSheet.get(); }
    int getWidth() const { return _width; }
    int getHeight() const { return _height; }

private:
    std::unique_ptr<SDL_Window, SDL_Deleter> _window;
    std::unique_ptr<SDL_Renderer, SDL_Deleter> _renderer;
    std::unique_ptr<SpriteSheet> _spriteSheet;
    std::unique_ptr<SpriteSheet> _playerSpriteSheet;

    int _width;
    int _height;
};