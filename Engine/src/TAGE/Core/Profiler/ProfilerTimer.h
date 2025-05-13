#pragma once

#include <chrono>  
#include <thread>  

namespace TAGE::DEBUG {
    class ProfilerTimer {
    public:
        ProfilerTimer(const std::string& name)
            : m_Name(name), m_Stopped(false) {
            m_StartTimepoint = std::chrono::high_resolution_clock::now();
        }

        ~ProfilerTimer() {
            if (!m_Stopped)
                Stop();
        }

        void Stop();

    private:
        std::string m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimepoint;
        bool m_Stopped;
    };
}