#include <map>
#include <cmath>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include <SDL2/SDL.h>

#include <nlohmann/json.hpp>

#include "game.hpp"

using json = nlohmann::json;

int main()
{
    try
    {
        Game game(config::window::width, config::window::height);
        game.run();
    }
    catch (const std::exception &e)
    {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
// #include <iostream>

// int main(int argc, char *argv[])
// {
//     if (SDL_Init(SDL_INIT_VIDEO) != 0)
//     {
//         std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
//         return 1;
//     }

//     if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
//     {
//         std::cerr << "IMG Init Error: " << IMG_GetError() << std::endl;
//         return 1;
//     }

//     SDL_Window *window = SDL_CreateWindow(
//         "Sprite Test",
//         SDL_WINDOWPOS_CENTERED,
//         SDL_WINDOWPOS_CENTERED,
//         400,
//         300,
//         SDL_WINDOW_SHOWN);

//     SDL_Renderer *renderer = SDL_CreateRenderer(
//         window,
//         -1,
//         SDL_RENDERER_ACCELERATED);

//     SDL_Surface *surface = IMG_Load("/Users/lazycodebaker/Documents/Dev/CPP/platformer_2d/assets/map/spritesheet.png");
//     if (!surface)
//     {
//         std::cerr << "Image Load Error: " << IMG_GetError() << std::endl;
//         return 1;
//     }

//     SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
//     SDL_FreeSurface(surface);

//     // --- Source rectangle (first sprite) ---
//     int tileIndex = 44; // 2nd tile
//     int tileSize = 16;
//     int tilesPerRow = 8; // 128 / 16

//     SDL_Rect srcRect;
//     srcRect.x = (tileIndex % tilesPerRow) * tileSize;
//     srcRect.y = (tileIndex / tilesPerRow) * tileSize;
//     srcRect.w = tileSize;
//     srcRect.h = tileSize;

//     // --- Destination rectangle (on screen) ---
//     SDL_Rect dstRect;
//     dstRect.x = 100;
//     dstRect.y = 100;
//     dstRect.w = 32; // scaled up for visibility
//     dstRect.h = 32;

//     bool running = true;
//     SDL_Event event;

//     while (running)
//     {
//         while (SDL_PollEvent(&event))
//         {
//             if (event.type == SDL_QUIT)
//                 running = false;
//         }

//         SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
//         SDL_RenderClear(renderer);

//         SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);

//         SDL_RenderPresent(renderer);
//     }

//     SDL_DestroyTexture(texture);
//     SDL_DestroyRenderer(renderer);
//     SDL_DestroyWindow(window);

//     IMG_Quit();
//     SDL_Quit();

//     return 0;
// }
