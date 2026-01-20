#pragma once

namespace config
{
    namespace window
    {
        inline constexpr int width = 1280;
        inline constexpr int height = 640;
    }

    namespace game
    {
        inline constexpr float target_fps = 60.0f;
        inline constexpr float frame_time = 1.0f / target_fps;
    }

    namespace physics
    {
        inline constexpr float player_speed = 300.0f;
        inline constexpr float jump_force = 600.0f;
        inline constexpr float gravity = 981.0f;
    }

    namespace tile
    {
        inline constexpr int tiles_per_row = 9;
        inline constexpr int size = 16;
        inline constexpr int map_width = 38;
        inline constexpr int map_height = size * 10;
        inline constexpr float scale = 2.0f;

        inline constexpr int scaled_size = int(size * scale);

        inline constexpr int screen_tiles_x = config::window::width / scaled_size;
        inline constexpr int screen_tiles_y = config::window::height / scaled_size;

    }
};

// cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON && cmake --build build
