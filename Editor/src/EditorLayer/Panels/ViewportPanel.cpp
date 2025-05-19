#include "ViewportPanel.h"

namespace TAGE::EDITOR {
	void ViewportPanel::SetContext(const MEM::Ref<Scene>& context)
	{
		m_Context = context;
	}
	void ViewportPanel::OnImGuiRender()
	{
		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		ImGui::Image((ImTextureID)(void*)GetMainFramebuffer()->GetColorAttachment(), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));

		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_ITEM"))
			{
				const wchar_t* assetPath = (const wchar_t*)payload->Data;
				if (assetPath)
				{
					std::filesystem::path path = std::filesystem::path(assetPath); 
					if (path.extension() == ".fbx" || path.extension() == ".obj" || path.extension() == ".gltf")
					{
						TEntity* entity = m_Context->CreateEntity(path.filename().string());
						entity->AddComponent<StaticMeshComponent>((PROJECT::ProjectCreator::GetRootPath() / "Content" / path).string());
					}
				}
			}
			ImGui::EndDragDropTarget();
		}

		ImGui::End();
	}
}