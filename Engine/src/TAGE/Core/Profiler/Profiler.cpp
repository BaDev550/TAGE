#include "tagepch.h"
#include "Profiler.h"

namespace TAGE::DEBUG {
    void Profiler::BeginSession(const std::string& name) {
        m_Entries.clear();
        AddEntry("Session Started: " + name, 0.0f);
    }

    void Profiler::EndSession() {
        AddEntry("Session Ended", 0.0f);
    }

    void Profiler::AddEntry(const std::string& name, float timeMs) {
        for (auto& entry : m_Entries) {
            if (entry.Name == name) {
                entry.TimeInMs = timeMs;
                return;
            }
        }
        m_Entries.emplace_back(ProfilerEntry{ name, timeMs });
    }
}