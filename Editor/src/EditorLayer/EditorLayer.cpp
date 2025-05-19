#include "EditorLayer.h"

namespace TAGE::EDITOR {
	EditorLayer::EditorLayer()
	{
		GetMainWindow().EnableCursor(true);
		GetMainWindow().SetWindowIcon("../TAGE_Logo.png");
	}

	void EditorLayer::SetDockspace()
	{
		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoCollapse |
			ImGuiWindowFlags_NoResize |
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoBringToFrontOnFocus |
			ImGuiWindowFlags_NoNavFocus;

		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

		ImGui::Begin("TAGE Level Editor", nullptr, window_flags);

		ImGui::PopStyleVar(2);

		ImGuiID dockspace_id = ImGui::GetID("MainDockSpace");

		if (!ImGui::DockBuilderGetNode(dockspace_id))
		{
			ImGui::DockBuilderRemoveNode(dockspace_id);
			ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
			ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

			ImGuiID dock_main_id = dockspace_id;

			ImGuiID dock_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.30f, nullptr, &dock_main_id);
			ImGuiID dock_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);
			ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);
			ImGuiID dock_bottom_right = ImGui::DockBuilderSplitNode(dock_right, ImGuiDir_Down, 0.50f, nullptr, &dock_right);
			ImGuiID dock_bottom_left = ImGui::DockBuilderSplitNode(dock_left, ImGuiDir_Down, 0.50f, nullptr, &dock_left);

			ImGui::DockBuilderDockWindow("Scene Hierarchy", dock_left);
			ImGui::DockBuilderDockWindow("Inspector", dock_bottom_left);
			ImGui::DockBuilderDockWindow("Output", dock_bottom);
			ImGui::DockBuilderDockWindow("Content Browser", dock_bottom);
			ImGui::DockBuilderDockWindow("Viewport", dock_main_id);

			ImGui::DockBuilderFinish(dockspace_id);
		}
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_PassthruCentralNode);

		ImGui::End();
	}

	void EditorLayer::OnAttach()
	{
		_EditorCamera = MEM::CreateRef<TEEditorCamera>(45.0f, 1.0f, 0.1f, 1000.0f);
		GAMEFACTORY::GameInstance::Get()->GetCameraManager().SetEditorCamera(_EditorCamera);
		GAMEFACTORY::GameInstance::Get()->GetCameraManager().UseEditorCamera();

		_SceneHierarchyPanel.SetContext(GetScene());
		_ViewportPanel.SetContext(GetScene());

		_SceneHierarchyPanel.SetOnEntitySelectedCallback([this](Entity entity) {
			_InspectorPanel.SetSelectedEntity(entity);
			});

		_ContentBrowser.SetProjectDirectory(PROJECT::ProjectCreator::GetRootPath() / "Content");
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnImGuiRender()
	{
		SetDockspace();
		_ToolBarPanel.OnImGuiRender();
		_SceneHierarchyPanel.OnImGuiRender();
		_InspectorPanel.OnImGuiRender();
		_ViewportPanel.OnImGuiRender();
		_OutputPanel.OnImGuiRender();
		_ContentBrowser.OnImGuiRender();
	}

	void EditorLayer::OnScenePlay()
	{
	}

	void EditorLayer::OnSceneStop()
	{
	}
}