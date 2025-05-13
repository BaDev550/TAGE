#pragma once

#include "TAGE/Layers/Layer.h"
#include "Profiler.h"
#include "imgui.h"

namespace TAGE::DEBUG {
    class ProfilerLayer : public Layer {
    public:
        ProfilerLayer() : Layer("Profiler") {}

        void OnImGuiRender() override {
            ImGui::Begin("Profiler");

            auto& profiler = Profiler::Get();
            const auto& entries = profiler.GetEntries();

            float mainLoopTime = 0.0f;

            for (const auto& entry : entries) {
                ImGui::Text("%s: %.3f ms", entry.Name.c_str(), entry.TimeInMs);

                if (entry.Name == "MainLoop") {
                    mainLoopTime = entry.TimeInMs;
                }
            }

            m_MainLoopHistory.push_back(mainLoopTime);
            if (m_MainLoopHistory.size() > m_HistorySize)
                m_MainLoopHistory.pop_front();

            if (!m_MainLoopHistory.empty()) {
                std::vector<float> plotData(m_MainLoopHistory.begin(), m_MainLoopHistory.end());
                ImGui::PlotLines("MainLoop Time", plotData.data(), static_cast<int>(plotData.size()), 0, nullptr, 0.0f, 200.0f, ImVec2(0, 80));
            }

            ImGui::End();
        }
    private:
        std::deque<float> m_MainLoopHistory;
        size_t m_HistorySize = 100;
    };
}