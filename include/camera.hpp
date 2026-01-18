#pragma once

#include "vector2f.hpp"

class Camera
{
public:
    Camera(int viewportWidth, int viewportHeight);
    ~Camera() = default;

    void follow(const Vector2f &targetPos, int targetWidth, int targetHeight, int mapWidth, int mapHeight);

    void moveHorizontal(float dx, int mapWidth);
    void updateCamera(float deltaTime, int mapWidth);

    Vector2f getPosition() const { return _position; }

    int getViewportWidth() const { return _viewportWidth; }
    int getViewportHeight() const { return _viewportHeight; }

    Vector2f worldToScreen(const Vector2f &worldPos) const;

private:
    Vector2f _position;
    int _viewportWidth;
    int _viewportHeight;
};