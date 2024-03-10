#include "Logger.h"
#include "RoxEngine/Core/Thread.h"
#include <atomic>
#include <memory>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <thread>
#include <sstream>

namespace RoxEngine {
	// Thread: Name (id)
	// Thread: id
	class ThreadNameFlag : public spdlog::custom_flag_formatter
	{
	public:
		void format(const spdlog::details::log_msg &, const std::tm &, spdlog::memory_buf_t &dest) override
		{
			std::stringstream ss;
			std::string thread_name = ThreadName();

			ss << "Thread: ";
			if(thread_name.size() == 0) {
				ss << std::this_thread::get_id();
			} else {
				ss << "\"" << thread_name << "\" (" << std::this_thread::get_id() << ")";
			}

			std::string str = ss.str();
			dest.append(str.data(), str.data() + str.size());
		}

		std::unique_ptr<custom_flag_formatter> clone() const override
		{
			return spdlog::details::make_unique<ThreadNameFlag>();
		}
	};

    Logger::Logger() {
        std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("log.txt", true));

		auto formatter = std::make_unique<spdlog::pattern_formatter>();
    	formatter->add_flag<ThreadNameFlag>('*').set_pattern("%^[%T] [%*] %n: %v%$");

		logSinks[0]->set_formatter(formatter->clone());
		formatter->set_pattern("[%T] [%*] [%l] %n: %v");
		logSinks[1]->set_formatter(std::move(formatter));

		mCoreLogger = std::make_shared<spdlog::logger>("ROXENGINE", begin(logSinks), end(logSinks));
		spdlog::register_logger(mCoreLogger);
		mCoreLogger->set_level(spdlog::level::trace);
		mCoreLogger->flush_on(spdlog::level::trace);

		mClientLogger = std::make_shared<spdlog::logger>("GAME", begin(logSinks), end(logSinks));
		spdlog::register_logger(mClientLogger);
		mClientLogger->set_level(spdlog::level::trace);
		mClientLogger->flush_on(spdlog::level::trace);
    }
    std::shared_ptr<Logger> Logger::Get() {
        static std::shared_ptr<Logger> logger = std::make_shared<Logger>();
        return logger;
    }
}