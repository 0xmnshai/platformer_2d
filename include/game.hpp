#pragma once

#include <vector>
#include <memory>

#include <SDL2/SDL.h>

#include "config.hpp"
#include "sdl_context.hpp" 
#include "renderer.hpp"
#include "input_handler.hpp"
#include "spritesheet.hpp"
#include "tilemap.hpp"
#include "player.hpp"

class Game
{
public:
    Game(int width = config::window::width, int height = config::window::height);
    ~Game() = default;

    Game(const Game &) = delete;
    Game &operator=(const Game &) = delete;

    void run();

private:
    void update(float deltaTime);
    void render();
    void createMap();
    void createComplexLevel();
    void createExtendedLevel();

    SDL_Context _sdlContext;
    std::unique_ptr<Renderer> _renderer;

    std::unique_ptr<Camera> _camera;
    std::unique_ptr<InputHandler> _inputHandler;

    // Textures
    std::unique_ptr<SDL_Texture, SDL_Deleter> _grassTexture;
    // std::unique_ptr<SDL_Texture, SDLDeleter> dirtTexture;
    // std::unique_ptr<SDL_Texture, SDLDeleter> platformTexture;

    // TileMap
    std::unique_ptr<TileMap> _tileMap;

    // Player
    std::unique_ptr<Player> _player;
    Vector2f spawnPoint;

    // Moving platforms
    // std::vector<std::unique_ptr<MovingPlatform>> movingPlatforms;

    // Moving obstacles
    // std::vector<std::unique_ptr<MovingObstacle>> movingObstacles;

    // Timing
    Uint64 _lastFrameTime;
};
