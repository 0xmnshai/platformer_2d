
#include "utils.hpp"

float Utils::clamp(float value, float min, float max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

Uint64 Utils::getCurrentTimeInMilliseconds()
{
    return SDL_GetTicks64();
}

float Utils::calculateDeltaTime(Uint64 lastTime, Uint64 currentTime)
{
    return static_cast<float>(currentTime - lastTime) / 1000.0f;
}

float Utils::getDistanceBetweenPoints(const Vector2f &p1, const Vector2f &p2)
{
    return (p1 - p2).length();
}

float Utils::calculateAngleBetweenPoints(const Vector2f &p1, const Vector2f &p2)
{
    Vector2f diff = p2 - p1;
    return std::atan2(diff.y, diff.x);
}
