#pragma once

#include "Panel.h"
#include "TAGE/TAGE.h"
#include "TAGE/TComponents.h"

namespace TAGE::EDITOR {
    class InspectorPanel : public Panel {
    public:
        InspectorPanel() = default;

        void SetSelectedEntity(Entity entity);
        void OnImGuiRender() override;

        template<typename T, typename UIFunction>
        void DrawComponent(const std::string& name, UIFunction uiFunction);
    private:
        Entity m_SelectedEntity;
    };

    template<typename T, typename UIFunction>
    inline void InspectorPanel::DrawComponent(const std::string& name, UIFunction uiFunction)
    {
        if (m_SelectedEntity.HasComponent<T>()) {
            auto& component = m_SelectedEntity.GetComponent<T>();

            ImGui::PushID(name.c_str());
            if (ImGui::TreeNodeEx((void*)typeid(T).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, name.c_str())) {
                uiFunction(component);
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }
    template void InspectorPanel::DrawComponent<TTransformComponent>(const std::string&, std::function<void(TTransformComponent&)>);
    template void InspectorPanel::DrawComponent<TStaticMeshComponent>(const std::string&, std::function<void(TStaticMeshComponent&)>);
}
