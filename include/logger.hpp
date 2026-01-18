#pragma once

#include <memory>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

class Logger
{
public:
    static void Init();

    inline static std::shared_ptr<spdlog::logger> &GetCoreLogger() { return s_CoreLogger; }
    inline static std::shared_ptr<spdlog::logger> &GetClientLogger() { return s_ClientLogger; }

private:
    static std::shared_ptr<spdlog::logger> s_CoreLogger;
    static std::shared_ptr<spdlog::logger> s_ClientLogger;
};

// If we define "DIST" (Distribution build), we can disable logging to save CPU
#ifdef DIST
#define CORE_TRACE(...)
#define CORE_INFO(...)
#define CORE_WARN(...)
#define CORE_ERROR(...)
#define CORE_CRITICAL(...)

#define LOG_TRACE(...)
#define LOG_INFO(...)
#define LOG_WARN(...)
#define LOG_ERROR(...)
#define LOG_CRITICAL(...)
#else
// Core Log Macros (For Engine/System code)
#define CORE_TRACE(...) ::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...) ::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...) ::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) ::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// Client Log Macros (For Game Logic/Player code)
#define LOG_TRACE(...) ::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) ::Logger::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) ::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) ::Logger::GetClientLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...) ::Logger::GetClientLogger()->critical(__VA_ARGS__)
#endif

// Stops the program if the condition is false
#ifdef ENABLE_ASSERTS
#define CORE_ASSERT(x, ...)                                   \
    {                                                         \
        if (!(x))                                             \
        {                                                     \
            CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __builtin_trap();                                 \
        }                                                     \
    }
#define LOG_ASSERT(x, ...)                                   \
    {                                                        \
        if (!(x))                                            \
        {                                                    \
            LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            __builtin_trap();                                \
        }                                                    \
    }
#else
#define CORE_ASSERT(x, ...)
#define LOG_ASSERT(x, ...)
#endif