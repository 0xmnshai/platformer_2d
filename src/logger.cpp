#include "logger.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

#include <vector>

std::shared_ptr<spdlog::logger> Logger::s_CoreLogger;
std::shared_ptr<spdlog::logger> Logger::s_ClientLogger;

void Logger::Init()
{
    std::vector<spdlog::sink_ptr> logSinks;

    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();

    consoleSink->set_pattern("%^[%T] %n: %v%$");
    logSinks.emplace_back(consoleSink);

    auto fileSink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("platformer.log", true);

    fileSink->set_pattern("[%Y-%m-%d %T] [%t] [%l] %n: %v");
    logSinks.emplace_back(fileSink);

    s_CoreLogger = std::make_shared<spdlog::logger>("ENGINE", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_CoreLogger);
    s_CoreLogger->set_level(spdlog::level::trace);
    s_CoreLogger->flush_on(spdlog::level::trace);

    s_ClientLogger = std::make_shared<spdlog::logger>("GAME", begin(logSinks), end(logSinks));
    spdlog::register_logger(s_ClientLogger);
    s_ClientLogger->set_level(spdlog::level::trace);
    s_ClientLogger->flush_on(spdlog::level::trace);
}