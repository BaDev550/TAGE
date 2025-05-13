#include "tagepch.h"
#include "ProfilerTimer.h"
#include "Profiler.h"

void TAGE::DEBUG::ProfilerTimer::Stop()
{
	auto endTimepoint = std::chrono::high_resolution_clock::now();
	float duration = std::chrono::duration<float, std::milli>(endTimepoint - m_StartTimepoint).count();
	Profiler::Get().AddEntry(m_Name, duration);
	m_Stopped = true;
}
