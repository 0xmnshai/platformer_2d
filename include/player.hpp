#pragma once

#include <SDL2/SDL.h>

#include "vector2f.hpp"
#include "entity.hpp"
#include "config.hpp"

#include "tilemap.hpp"
#include "spritesheet.hpp"

class Player : public Entity
{
public:
    Player(Vector2f position, int width, int height, TileMap *tileMap, SpriteSheet *spriteSheet);
    ~Player() = default;

    void update(float deltaTime) override;
    void render(SDL_Renderer *renderer) override;

    void moveLeft()
    {
        _velocity.x = -config::physics::player_speed;
        _facingRight = false;
    }
    
    void moveRight()
    {
        _velocity.x = config::physics::player_speed;
        _facingRight = true;
    }

    void stopHorizontal() { _velocity.x = 0; }

    void jump();

    bool isGrounded() const { return _grounded; }
    bool isAlive() const { return _alive; }

    void respawn(const Vector2f &pos)
    {
        _position = pos;
        _velocity = Vector2f(0, 0);
        _alive = true;
        _platformVelocity = Vector2f(0, 0);
    }

    bool checkMovingPlatformCollision(const Vector2f &platformPos, int platformWidth, int platformHeight);
    bool checkMovingObstacleCollision(const Vector2f &obstaclePos, int obstacleWidth, int obstacleHeight);
    void applyPlatformVelocity(const Vector2f &velocity) { _platformVelocity = velocity; }

private:
    bool _grounded;
    bool _alive;
    bool _facingRight;
    TileMap *_tileMap;
    SpriteSheet *_spriteSheet;
    Vector2f _platformVelocity;

    void checkCollisions();
    bool isCollidingWith(const Vector2f &pos, int w, int h);
};