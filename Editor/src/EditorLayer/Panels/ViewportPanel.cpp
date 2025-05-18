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

		ImGui::End();
	}
}