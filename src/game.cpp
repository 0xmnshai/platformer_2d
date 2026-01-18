#include <iostream>
#include <stdexcept>
#include <map>

#include "game.hpp"
#include "renderer.hpp"
#include "input_handler.hpp"
#include "utils.hpp"
#include "spritesheet.hpp"
#include "logger.hpp"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Game::Game(int width, int height)
    : _lastFrameTime(SDL_GetTicks64()),
      _inputHandler(std::make_unique<InputHandler>())
{
    Logger::Init();

    CORE_WARN("Engine Initializing...");

    _renderer = std::make_unique<Renderer>(
        "SDL2 Platformer 2D", width, height);
    _camera = std::make_unique<Camera>(width, height);

    const std::string spritesheetPath = "assets/map/spritesheet.png";
    const std::string playerSpritesheetPath = "assets/player/spritesheet.png";

    try
    {
        _renderer->loadSpriteSheet(spritesheetPath);
        _renderer->loadPlayerSpriteSheet(playerSpritesheetPath);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: Could not load spritesheet: " << e.what() << std::endl;
        throw;
    }

    _tileMap = std::make_unique<TileMap>(
        _renderer->getSpriteSheet(),
        config::tile::map_width,   // config::window::width / config::tile::size,
        config::tile::map_height); // config::window::height / config::tile::size);

    spawnPoint = Vector2f(0, 0);
    _player = std::make_unique<Player>(spawnPoint, 64, 64, _tileMap.get(), _renderer->getPlayerSpriteSheet());

    _tileMap->loadFromJSON("assets/map/map.json");

    CORE_WARN("Engine Initialized");
}

void Game::update(float deltaTime)
{
    if (_inputHandler->isKeyDown(SDL_SCANCODE_LEFT))
    {
        _player->moveLeft();
    }
    else if (_inputHandler->isKeyDown(SDL_SCANCODE_RIGHT))
    {
        _player->moveRight();
    }
    else
    {
        _player->stopHorizontal();
    }

    if (_inputHandler->isKeyDown(SDL_SCANCODE_SPACE))
    {
        _player->jump();
    }

    _player->update(deltaTime);

    bool freeLook = true;

    if (freeLook)
    {
        _camera->updateCamera(deltaTime, _tileMap->getMapWidth());
    }
    else
    {
        _camera->follow(_player->getPosition(), _player->getWidth(), _player->getHeight(),
                        _tileMap->getMapWidth(), _tileMap->getMapHeight());
    }
}

void Game::render()
{
    _renderer->clear();
    _player->render(_renderer->getSDLRenderer());

    if (_tileMap)
    {
        _renderer->drawTileMapWithCamera(*_tileMap, *_camera);
    }

    _renderer->present();
}

void Game::run()
{
    while (_inputHandler->isRunning())
    {
        Uint64 currentTime = SDL_GetTicks64();
        float deltaTime = Utils::calculateDeltaTime(
            _lastFrameTime, currentTime);

        _lastFrameTime = currentTime;

        if (deltaTime > 0.05f)
            deltaTime = 0.05f;

        _inputHandler->update();
        update(deltaTime);
        render();
    }
}
