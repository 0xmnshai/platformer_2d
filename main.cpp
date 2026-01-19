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