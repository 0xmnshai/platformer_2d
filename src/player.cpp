
#include "player.hpp"
#include "config.hpp"

Player::Player(Vector2f position, int width, int height, TileMap *tileMap, SpriteSheet *spriteSheet)
    : Entity(position, width, height, nullptr), _grounded(false), _alive(true), _facingRight(true), _tileMap(tileMap), _spriteSheet(spriteSheet), _platformVelocity(0, 0) {};

void Player::jump()
{
    if (_grounded)
    {
        _velocity.y = -config::physics::jump_force;
        _grounded = false;
    }
}

bool Player::isCollidingWith(const Vector2f &pos, int w, int h)
{
    return !(_position.x + _width < pos.x || _position.x > pos.x + w ||
             _position.y + _height < pos.y || _position.y > pos.y + h);
}

void Player::checkCollisions()
{
    _grounded = false;
    const int TILE_SIZE = config::tile::size;

    // Check tile collisions
    int tileX1 = static_cast<int>(_position.x) / TILE_SIZE;
    int tileY1 = static_cast<int>(_position.y) / TILE_SIZE;
    int tileX2 = static_cast<int>(_position.x + _width - 1) / TILE_SIZE;
    int tileY2 = static_cast<int>(_position.y + _height - 1) / TILE_SIZE;

    for (int y = tileY1; y <= tileY2; ++y)
    {
        for (int x = tileX1; x <= tileX2; ++x)
        {
            const Tile *tile = _tileMap->getTile(x, y);
            if (tile && tile->isSolid())
            {
                // // Handle spike collision
                // if (tile->getType() == Tile::TileType::SURFACE)
                // {
                //     _alive = false;
                //     return;
                // }

                Vector2f tilePos = tile->getWorldPosition();
                float tileLeft = tilePos.x;
                float tileRight = tilePos.x + TILE_SIZE;
                float tileTop = tilePos.y;
                float tileBottom = tilePos.y + TILE_SIZE;

                float playerLeft = _position.x;
                float playerRight = _position.x + _width;
                float playerTop = _position.y;
                float playerBottom = _position.y + _height;

                // Calculate overlap on each side
                float overlapLeft = playerRight - tileLeft;
                float overlapRight = tileRight - playerLeft;
                float overlapTop = playerBottom - tileTop;
                float overlapBottom = tileBottom - playerTop;

                // Find the minimum overlap to determine collision side
                float minOverlap = std::min({overlapLeft, overlapRight, overlapTop, overlapBottom});

                // Bottom collision (player standing on tile)
                if (minOverlap == overlapTop && _velocity.y >= 0)
                {
                    _position.y = tileTop - _height;
                    _velocity.y = 0;
                    _grounded = true;
                }

                // Top collision (player head hitting tile)
                else if (minOverlap == overlapBottom && _velocity.y < 0)
                {
                    _position.y = tileBottom;
                    _velocity.y = 0;
                }
                // Left collision (player hitting tile from left)
                else if (minOverlap == overlapLeft && _velocity.x > 0)
                {
                    _position.x = tileLeft - _width;
                    _velocity.x = 0;
                }
                // Right collision (player hitting tile from right)
                else if (minOverlap == overlapRight && _velocity.x < 0)
                {
                    _position.x = tileRight;
                    _velocity.x = 0;
                }
            }
        }
    }
}

void Player::update(float deltaTime)
{
    _velocity.y += config::physics::gravity * deltaTime;
    _position += _velocity * deltaTime;

    checkCollisions();
}

void Player::render(SDL_Renderer *renderer)
{
    if (!_spriteSheet || !_spriteSheet->getTexture())
        return;

    // Select sprite based on state
    // Sprite sprite = (!_grounded) ? (_facingRight ? SpriteSheet::PLAYER_JUMP_RIGHT : SpriteSheet::PLAYER_JUMP_LEFT) : (facingRight ? SpriteSheet::PLAYER_IDLE_RIGHT : SpriteSheet::PLAYER_IDLE_LEFT);

    // using that enemy as player for now : col : 3 , row , 6

    Sprite sprite = SpriteSheet::PLAYER_IDLE_LEFT;

    const int TILE_SIZE = config::tile::size;
    int scaledTileSize = TILE_SIZE * 2; // config::tile::scale;

    int src_x = (sprite.index % config::tile::tiles_per_row) * config::tile::size;
    int src_y = (sprite.index / config::tile::tiles_per_row) * config::tile::size;

    SDL_Rect srcRect = {src_x, src_y, config::tile::size, config::tile::size};

    SDL_Rect dstRect = {static_cast<int>(_position.x), static_cast<int>(_position.y), scaledTileSize, scaledTileSize};
    SDL_RenderCopyEx(renderer, _spriteSheet->getTexture(), &srcRect, &dstRect, 0, nullptr, _facingRight ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL);
}