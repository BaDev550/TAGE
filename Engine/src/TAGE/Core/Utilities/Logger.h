#pragma once

#include "TAGE/Core/Utilities/Memory.h"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#include "spdlog/fmt/fmt.h"
#include <iostream>

#define CREATE_LOGGER() TAGE::Log::Init();
#define CORE_LOG_INFO(...) TAGE::Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_LOG_WARN(...) TAGE::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_LOG_ERROR(...)TAGE::Log::GetCoreLogger()->error(__VA_ARGS__)

#define LOG_INFO(...) TAGE::Log::GetClientLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) TAGE::Log::GetClientLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...)TAGE::Log::GetClientLogger()->error(__VA_ARGS__)

namespace TAGE {
	class Log
	{
	public:
		Log() {}
		~Log() {}
		static void Init() {
			spdlog::set_pattern("%^[%l] %T %n: %v%$");
			s_CoreLogger = spdlog::stdout_color_mt("CORE");
			s_CoreLogger->set_level(spdlog::level::trace);

			s_ClientLogger = spdlog::stdout_color_mt("APP");
			s_ClientLogger->set_level(spdlog::level::trace);
		}

		inline static MEM::Ref<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static MEM::Ref<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static MEM::Ref<spdlog::logger> s_CoreLogger;
		static MEM::Ref<spdlog::logger> s_ClientLogger;
	};
}