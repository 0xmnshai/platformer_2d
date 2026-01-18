
#include "renderer.hpp"
#include "sdl_deleter.hpp"
#include "tilemap.hpp"

Renderer::Renderer(const std::string &title, int width, int height)
    : _width(width), _height(height)
{
    _window = std::unique_ptr<SDL_Window, SDL_Deleter>(
        SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN));

    if (!_window)
    {
        throw std::runtime_error("Failed to create window");
    };

    _renderer = std::unique_ptr<SDL_Renderer, SDL_Deleter>(SDL_CreateRenderer(_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

    if (!_renderer)
    {
        throw std::runtime_error("Failed to create renderer");
    };
};

Renderer::~Renderer() = default;

void Renderer::clear(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(_renderer.get(), r, g, b, a);
    SDL_RenderClear(_renderer.get());
};

void Renderer::present()
{
    SDL_RenderPresent(_renderer.get());
}

void Renderer::loadTexture(const std::string &filePath, std::unique_ptr<SDL_Texture, SDL_Deleter> &outTexture)
{
    SDL_Surface *surface = IMG_Load(filePath.c_str());
    if (!surface)
    {
        throw std::runtime_error(std::string("Failed to load image: ") + IMG_GetError());
    }

    SDL_Texture *tex = SDL_CreateTextureFromSurface(_renderer.get(), surface);
    SDL_FreeSurface(surface);

    if (!tex)
    {
        throw std::runtime_error(std::string("Failed to create texture: ") + SDL_GetError());
    }

    outTexture.reset(tex);
}

void Renderer::loadSpriteSheet(const std::string &filePath)
{
    _spriteSheet = std::make_unique<SpriteSheet>(_renderer.get(), filePath);
}

void Renderer::loadPlayerSpriteSheet(const std::string &filePath)
{
    _playerSpriteSheet = std::make_unique<SpriteSheet>(_renderer.get(), filePath);
}

void Renderer::drawTileMapWithCamera(const TileMap &tileMap, const Camera &camera)
{
    tileMap.renderWithCamera(_renderer.get(), camera);
}

void Renderer::drawEntity(const Entity &entity)
{
    SDL_Rect srcRect = {0, 0, entity.getWidth(), entity.getHeight()};
    SDL_Rect dstRect = {static_cast<int>(entity.getPosition().x), static_cast<int>(entity.getPosition().y),
                        entity.getWidth(), entity.getHeight()};
    SDL_RenderCopy(_renderer.get(), entity.getTexture(), &srcRect, &dstRect);
}

void Renderer::drawTileMap(const TileMap &tileMap)
{
    tileMap.render(_renderer.get());
}

void Renderer::drawRect(int x, int y, int w, int h, Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
    SDL_SetRenderDrawColor(_renderer.get(), r, g, b, a);
    SDL_Rect rect = {x, y, w, h};
    SDL_RenderDrawRect(_renderer.get(), &rect);
}

void Renderer::drawSprite(const Sprite &sprite, int x, int y, int w, int h)
{
    if (!_spriteSheet)
        return;

    int src_x = (sprite.index % config::tile::tiles_per_row) * config::tile::size;
    int src_y = (sprite.index / config::tile::tiles_per_row) * config::tile::size;

    SDL_Rect srcRect = {src_x, src_y, config::tile::size, config::tile::size};

    SDL_Rect dstRect = {x, y, w, h};
    SDL_RenderCopy(_renderer.get(), _spriteSheet->getTexture(), &srcRect, &dstRect);
}

void Renderer::drawPlayerSprite(const Sprite &sprite, int x, int y, int w, int h)
{
    if (!_playerSpriteSheet)
        return;
        
    int src_x = (sprite.index % config::tile::tiles_per_row) * config::tile::size;
    int src_y = (sprite.index / config::tile::tiles_per_row) * config::tile::size;

    SDL_Rect srcRect = {src_x, src_y, config::tile::size, config::tile::size};

    SDL_Rect dstRect = {x, y, w, h};
    SDL_RenderCopy(_renderer.get(), _playerSpriteSheet->getTexture(), &srcRect, &dstRect);
}

float Renderer::getRefreshRate() const
{
    SDL_DisplayMode displayMode;
    if (SDL_GetCurrentDisplayMode(0, &displayMode) != 0)
    {
        throw std::runtime_error(std::string("Failed to get display mode: ") + SDL_GetError());
    }
    return static_cast<float>(displayMode.refresh_rate);
}
