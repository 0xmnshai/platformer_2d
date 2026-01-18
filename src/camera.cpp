
#include <SDL2/SDL.h>

#include "camera.hpp"
#include "config.hpp"

Camera::Camera(int viewportWidth, int viewportHeight)
    : _position(0, 0), _viewportWidth(viewportWidth), _viewportHeight(viewportHeight) {}

void Camera::follow(const Vector2f &targetPos, int targetWidth, int targetHeight, int mapWidth, int mapHeight)
{
    // Center camera horizontally on player
    float targetCenterX = targetPos.x + targetWidth / 2.0f;
    float targetCenterY = targetPos.y + targetHeight / 2.0f;

    _position.x = targetCenterX - _viewportWidth / 2.0f;
    // Position player at 1/6 from top of screen (shifting map to bottom)
    _position.y = targetCenterY - _viewportHeight / 6.0f;

    // Clamp to map bounds
    if (_position.x < 0)
        _position.x = 0;
    if (_position.x + _viewportWidth > mapWidth * config::tile::size)
        _position.x = mapWidth * config::tile::size - _viewportWidth;

    if (_position.y < 0)
        _position.y = 0;
    if (_position.y + _viewportHeight > mapHeight * config::tile::size)
        _position.y = mapHeight * config::tile::size - _viewportHeight;
}

Vector2f Camera::worldToScreen(const Vector2f &worldPos) const
{
    return Vector2f(worldPos.x - _position.x, worldPos.y - _position.y);
}

void Camera::moveHorizontal(float dx, int mapWidth)
{
    _position.x += dx;

    if (_position.x < 0)
        _position.x = 0;

    float maxX = mapWidth * config::tile::size - _viewportWidth;
    if (_position.x > maxX)
        _position.x = maxX;
}

void Camera::updateCamera(float deltaTime, int mapWidth)
{
    float dx = 0.0f;

    const float CAMERA_SPEED = 3.0f; // pixels per second

    const Uint8 *keyState = SDL_GetKeyboardState(nullptr);

    if (keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_A])
        dx -= CAMERA_SPEED * deltaTime;

    if (keyState[SDL_SCANCODE_RIGHT] || keyState[SDL_SCANCODE_D])
        dx += CAMERA_SPEED * deltaTime;

    if (dx != 0.0f)
        this->moveHorizontal(dx, mapWidth);
}
