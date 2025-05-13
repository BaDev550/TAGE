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

            for (const auto& entry : entries) {
                ImGui::Text("%s: %.3f ms", entry.Name.c_str(), entry.TimeInMs);

                if (m_Index < 100 && entry.Name == "MainLoop") {
                    m_History[m_Index++] = entry.TimeInMs;
                    if (m_Index >= 100) m_Index = 0;
                }
            }

            ImGui::PlotLines("MainLoop Time", m_History, IM_ARRAYSIZE(m_History), 0, nullptr, 0.0f, 50.0f, ImVec2(0, 80));

            ImGui::End();
        }
    private:
        float m_History[100] = {};
        int m_Index = 0;
    };
}