#pragma once

// This ignores all warnings inside the headers
//#define SPDLOG_HEADER_ONLY
#pragma warning(push,0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace RoxEngine {
	class Logger {
	public:
		static void Init();

		static std::shared_ptr<spdlog::logger> GetCoreLogger() { return sCoreLogger; }
		static std::shared_ptr<spdlog::logger> GetClientLogger() { return sClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> sCoreLogger;
		static std::shared_ptr<spdlog::logger> sClientLogger;
	};
}

//core log macros
#define RE_CORE_TRACE(...) ::RoxEngine::Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define RE_CORE_INFO(...)  ::RoxEngine::Logger::GetCoreLogger()->info(__VA_ARGS__)
#define RE_CORE_WARN(...)  ::RoxEngine::Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define RE_CORE_ERROR(...) ::RoxEngine::Logger::GetCoreLogger()->error(__VA_ARGS__)
#define RE_CORE_FATAL(...) ::RoxEngine::Logger::GetCoreLogger()->critical(__VA_ARGS__)

// client log macros
#define RE_TRACE(...) ::RoxEngine::Logger::GetClientLogger()->trace(__VA_ARGS__)
#define RE_INFO(...)  ::RoxEngine::Logger::GetClientLogger()->info(__VA_ARGS__)
#define RE_WARN(...)  ::RoxEngine::Logger::GetClientLogger()->warn(__VA_ARGS__)
#define RE_ERROR(...) ::RoxEngine::Logger::GetClientLogger()->error(__VA_ARGS__)
#define RE_FATAL(...) ::RoxEngine::Logger::GetClientLogger()->critical(__VA_ARGS__)