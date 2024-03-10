#pragma once

#include <memory>
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace RoxEngine {
    class Logger {
    public:
        Logger();
        static std::shared_ptr<Logger> Get();
        std::shared_ptr<spdlog::logger> mCoreLogger;
	    std::shared_ptr<spdlog::logger> mClientLogger; 
    };
}

// Engine log macros
#define CORE_TRACE(...)    ::RoxEngine::Logger::Get()->mCoreLogger->trace(__VA_ARGS__)
#define CORE_INFO(...)     ::RoxEngine::Logger::Get()->mCoreLogger->info(__VA_ARGS__)
#define CORE_WARN(...)     ::RoxEngine::Logger::Get()->mCoreLogger->warn(__VA_ARGS__)
#define CORE_ERROR(...)    ::RoxEngine::Logger::Get()->mCoreLogger->error(__VA_ARGS__)
#define CORE_CRITICAL(...) ::RoxEngine::Logger::Get()->mCoreLogger->critical(__VA_ARGS__)

// Client log macros
#define TRACE(...)         ::RoxEngine::Logger::Get()->mClientLogger->trace(__VA_ARGS__)
#define INFO(...)          ::RoxEngine::Logger::Get()->mClientLogger->info(__VA_ARGS__)
#define WARN(...)          ::RoxEngine::Logger::Get()->mClientLogger->warn(__VA_ARGS__)
#define ERROR(...)         ::RoxEngine::Logger::Get()->mClientLogger->error(__VA_ARGS__)
#define CRITICAL(...)      ::RoxEngine::Logger::Get()->mClientLogger->critical(__VA_ARGS__)