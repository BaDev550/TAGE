#pragma once

#include "Panel.h"
#include "TAGE/TAGE.h"

namespace TAGE::EDITOR {
    class ScenePanel : public Panel {
    public:
        ScenePanel() = default;
        ScenePanel(const MEM::Ref<Scene>& context);

        void SetContext(const MEM::Ref<Scene>& context);
        void OnImGuiRender() override;

        Entity GetSelectedEntity() const { return m_SelectionContext; }
        void SetSelectedEntity(Entity entity) { m_SelectionContext = entity; }

        std::function<void(Entity)> m_OnEntitySelected;
        void SetOnEntitySelectedCallback(const std::function<void(Entity)>& callback) { m_OnEntitySelected = callback; }
    private:
        MEM::Ref<Scene> m_Context;
        Entity m_SelectionContext;

        void DrawEntityNode(Entity entity);
    };
}