#include "InspectorPanel.h"
#include "TeGui.h"

namespace TAGE::EDITOR {
	void InspectorPanel::SetSelectedEntity(Entity entity)
	{
		m_SelectedEntity = entity;
	}

	void InspectorPanel::OnImGuiRender()
	{
        ImGui::Begin("Inspector");

        if (m_SelectedEntity.IsValid()) {
            if (m_SelectedEntity.HasComponent<TTagComponent>()) {
                auto& tag = m_SelectedEntity.GetComponent<TTagComponent>().name;
                char buffer[256];
                memset(buffer, 0, sizeof(buffer));
                strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));
                if (ImGui::InputText("Tag", buffer, sizeof(buffer))) {
                    tag = std::string(buffer);
                }
            }

            DrawComponent<TTransformComponent>("Transform", [](TTransformComponent& component) {
                TeGui::DrawVec3Control("Position", component.Position, 0.0f);
                TeGui::DrawVec3Control("Rotation", component.Rotation, 0.0f);
                TeGui::DrawVec3Control("Scale", component.Scale, 1.0f);
                });

            DrawComponent<TStaticMeshComponent>("Static Mesh Component", [](TStaticMeshComponent& component) {
                ImGui::Text("Path: %s", component.GetModel()->GetPath().c_str());
                });

            static std::vector<std::string> componentOptions = { "None", "Static Mesh", "Skeletal Mesh" };
            static int selectedIndex = 0;

            ImGui::Text("Add Component:");
            ImGui::PushItemWidth(-1);
            if (TeGui::ComboBox("Select...", componentOptions, selectedIndex)) {
                const std::string& selected = componentOptions[selectedIndex];

                if (selected == "Static Mesh" && !m_SelectedEntity.HasComponent<TStaticMeshComponent>())
                    m_SelectedEntity.AddComponent<TStaticMeshComponent>("Assets/Models/Cube/cube.obj");

                if (selected == "Skeletal Mesh" && !m_SelectedEntity.HasComponent<TSkeletalMeshComponent>())
                    m_SelectedEntity.AddComponent<TSkeletalMeshComponent>();

                selectedIndex = 0;
                ImGui::CloseCurrentPopup();
            }
            ImGui::PopItemWidth();
        }

        ImGui::End();
	}
}