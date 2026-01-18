#pragma once

#include <SDL2/SDL.h>

#include "vector2f.hpp"

class Utils
{
public:
    static float clamp(float value, float min, float max);
    static Uint64 getCurrentTimeInMilliseconds();
    static float calculateDeltaTime(Uint64 lastTime, Uint64 currentTime);
    static float getDistanceBetweenPoints(const Vector2f &p1, const Vector2f &p2);
    static float calculateAngleBetweenPoints(const Vector2f &p1, const Vector2f &p2);
};
