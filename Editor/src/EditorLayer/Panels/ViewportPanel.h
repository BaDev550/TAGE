#pragma once

#include "Panel.h"
#include "TAGE/TAGE.h"
#include "TAGE/TObjects.h"
#include "TAGE/TComponents.h"

namespace TAGE::EDITOR {
    class ViewportPanel : public Panel {
    public:
        void SetContext(const MEM::Ref<Scene>& context);
        void OnImGuiRender() override;

        bool IsFocused() const { return m_ViewportFocused; }
        bool IsHovered() const { return m_ViewportHovered; }

        glm::vec2 GetSize() const { return m_ViewportSize; }

    private:
        MEM::Ref<Scene> m_Context;
        glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
        bool m_ViewportFocused = false;
        bool m_ViewportHovered = false;
    };
}
