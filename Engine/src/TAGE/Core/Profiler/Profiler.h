#pragma once

#include <string>
#include <vector>
#include "TAGE/Core/Utilities/Timer.h"
#include "TAGE/Core/Profiler/ProfilerTimer.h"

#define TE_PROFILE_SCOPE(name) \
    TAGE::DEBUG::ProfilerTimer __timer##__LINE__(name)

namespace TAGE::DEBUG {
    struct ProfilerEntry {
        std::string Name;
        float TimeInMs;
    };

    class Profiler {
    public:
        static Profiler& Get() {
            static Profiler instance;
            return instance;
        }

        void BeginSession(const std::string& name);
        void EndSession();
        void AddEntry(const std::string& name, float timeMs);

        const std::vector<ProfilerEntry>& GetEntries() const { return m_Entries; }

    private:
        std::vector<ProfilerEntry> m_Entries;
    };
}
