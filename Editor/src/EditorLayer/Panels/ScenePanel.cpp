#include "ScenePanel.h"

namespace TAGE::EDITOR {
	ScenePanel::ScenePanel(const MEM::Ref<Scene>& context)
	{
		SetContext(context);
	}

	void ScenePanel::SetContext(const MEM::Ref<Scene>& context)
	{
		m_Context = context;
		m_SelectionContext = {};
	}

	void ScenePanel::OnImGuiRender()
	{
		ImGui::Begin("Scene Hierarchy");

		if (m_Context) {
			auto& registry = m_Context->GetRegistry();

			for (auto entity : m_Context->GetEntities())
				DrawEntityNode(*entity);

			if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered()) {
				m_SelectionContext = {};
				if (m_OnEntitySelected)
					m_OnEntitySelected({});
			}

			if (ImGui::BeginPopupContextWindow(0, 1)) {
				if (ImGui::MenuItem("Create Empty Entity"))
					m_Context->CreateEntity("Empty Entity");
				ImGui::EndPopup();
			}
		}

		ImGui::End();
	}

	void ScenePanel::DrawEntityNode(Entity entity)
	{
		auto& name = entity.GetComponent<TagComponent>().name;

		ImGuiTreeNodeFlags flags = ((m_SelectionContext == entity) ? ImGuiTreeNodeFlags_Selected : 0)
			| ImGuiTreeNodeFlags_OpenOnArrow
			| ImGuiTreeNodeFlags_SpanAvailWidth;

		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, name.c_str());
		if (ImGui::IsItemClicked())
		{
			m_SelectionContext = entity;
			if (m_OnEntitySelected)
				m_OnEntitySelected(entity);
		}


		bool entityDeleted = false;
		if (ImGui::BeginPopupContextItem()) {
			if (ImGui::MenuItem("Delete Entity")) {
				m_Context->DestroyEntity(entity);
				if (m_SelectionContext == entity)
					m_SelectionContext = {};
				entityDeleted = true;
			}
			ImGui::EndPopup();
		}

		if (opened) {
			ImGui::TreePop();
		}

		if (entityDeleted) return;
	}
}