
#include "entity.hpp"

Vector2f Entity::getCenter() const
{
    return Vector2f(_position.x + _width / 2.0f, _position.y + _height / 2.0f);
}

Entity::Entity(Vector2f position, int width, int height, SDL_Texture *texture)
    : _position(position), _width(width), _height(height), _texture(texture), _velocity(0, 0) {}

Entity::~Entity() = default;

void Entity::update(float deltaTime)
{
    _position += _velocity * deltaTime;
}

void Entity::render(SDL_Renderer *renderer)
{
    SDL_Rect srcRect = {0, 0, _width, _height};
    SDL_Rect dstRect = {static_cast<int>(_position.x), static_cast<int>(_position.y), _width, _height};
    SDL_RenderCopy(renderer, _texture, &srcRect, &dstRect);
}