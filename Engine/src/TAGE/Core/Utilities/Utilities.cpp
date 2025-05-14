#include "tagepch.h"
#include "Logger.h"
#include "Timer.h"
#include "Timeline.h"

namespace TAGE {
	MEM::Ref<spdlog::logger> Log::s_CoreLogger;
	MEM::Ref<spdlog::logger> Log::s_ClientLogger;
	std::vector<Timeline::Animation> Timeline::_Animations;

	Timer::Timer() {
		Reset();
	}
	void Timer::Reset() {
		_Start = std::chrono::high_resolution_clock::now();
	}
	float Timer::Elapsed() const {
		return std::chrono::duration<float>(std::chrono::high_resolution_clock::now() - _Start).count();
	}
	float Timer::ElapsedMillis() const {
		return Elapsed() * 1000.0f;
	}
}