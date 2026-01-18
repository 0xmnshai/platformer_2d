#pragma once

#include <SDL2/SDL.h>

#include "vector2f.hpp"

class Entity
{
public:
    Entity(Vector2f position, int width, int height, SDL_Texture *texture);
    virtual ~Entity();

    SDL_Texture *getTexture() const { return _texture; }

    Vector2f getPosition() const { return _position; }
    Vector2f getVelocity() const { return _velocity; }

    int getWidth() const { return _width; }
    int getHeight() const { return _height; }

    void setPosition(float x, float y) { _position = Vector2f(x, y); }
    void setPosition(const Vector2f &pos) { _position = pos; }

    void setVelocity(float x, float y) { _velocity = Vector2f(x, y); }
    void setVelocity(const Vector2f &vel) { _velocity = vel; }

    Vector2f getCenter() const;

    virtual void update(float deltaTime);
    virtual void render(SDL_Renderer *renderer);

protected:
    Vector2f _position;
    Vector2f _velocity;
    int _width;
    int _height;
    SDL_Texture *_texture;
};
