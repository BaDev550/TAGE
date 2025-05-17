#pragma once

#include "TAGE/Core/Utilities/Memory.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/fmt/fmt.h"
#include "LogSink.h"
#include <iostream>

#define CREATE_LOGGER() TAGE::Log::Init();
#define CORE_LOG_INFO(...) TAGE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...) TAGE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...)TAGE::Log::GetCoreLogger()->error(__VA_ARGS__)

#define LOG_INFO(...) TAGE::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) TAGE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)TAGE::Log::GetClientLogger()->error(__VA_ARGS__)

#define GET_MESSAGES() TAGE::Log::GetSink()->GetMessages();
#define CLEAR_MESSAGES() TAGE::Log::GetSink()->Clear();
namespace TAGE {
	class Log
	{
	public:
		Log() {}
		~Log() {}
		static void Init() {
			spdlog::set_pattern("%^[%l] %T %n: %v%$");

			s_Sink = MEM::CreateRef<LogSink>();

			auto core_sink = MEM::CreateRef<spdlog::sinks::stdout_color_sink_mt>();
			s_CoreLogger = MEM::CreateRef<spdlog::logger>("CORE", spdlog::sinks_init_list{ core_sink, s_Sink });
			spdlog::register_logger(s_CoreLogger);
			s_CoreLogger->set_level(spdlog::level::trace);

			auto client_sink = MEM::CreateRef<spdlog::sinks::stdout_color_sink_mt>();
			s_ClientLogger = MEM::CreateRef<spdlog::logger>("APP", spdlog::sinks_init_list{ client_sink, s_Sink });
			spdlog::register_logger(s_ClientLogger);
			s_ClientLogger->set_level(spdlog::level::trace);
		}

		inline static MEM::Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static MEM::Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
		inline static MEM::Ref<LogSink>& GetSink() { return s_Sink; }
	private:
		static MEM::Ref<spdlog::logger> s_CoreLogger;
		static MEM::Ref<spdlog::logger> s_ClientLogger;
		inline static MEM::Ref<LogSink> s_Sink;
	};
}