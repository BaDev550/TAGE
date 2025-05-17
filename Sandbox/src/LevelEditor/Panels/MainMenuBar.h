#pragma once
#include "imgui.h"
#include "TAGE/TAGE.h"
#include "Panel.h"

class MainMenuBar : public Panel {
public:
	MainMenuBar() {
		AddIcon("I_WireFrame", "Assets/Textures/Icons/WireframeCube.png");
		AddIcon("I_Fill", "Assets/Textures/Icons/FillCube.png");
		AddIcon("I_Close", "Assets/Textures/Icons/Close.png");
		AddIcon("I_Cube", "Assets/Textures/Icons/Cube.png");
		AddIcon("I_Line", "Assets/Textures/Icons/Line.png");
		AddIcon("IB_TAGE", "../Brand/TAGE_PNG.png");

		AddIcon("I_GTranslate", "Assets/Textures/Icons/Translate.png");
		AddIcon("I_GRotate", "Assets/Textures/Icons/Rotate.png");
		AddIcon("I_GScale", "Assets/Textures/Icons/Scale.png");

		AddIcon("I_Play", "Assets/Textures/Icons/Play.png");
		AddIcon("I_Pause", "Assets/Textures/Icons/Pause.png");
		AddIcon("I_Stop", "Assets/Textures/Icons/Stop.png");
	}
	virtual void Begin() override {
		static bool isDraggingWindow = false;
		static ImVec2 dragStartPos;
		if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New");
				ImGui::MenuItem("Open...");
				ImGui::MenuItem("Save");
				ImGui::Separator();
				if (ImGui::MenuItem("Exit"))
				{
					GetApp().Close();
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Undo");
				ImGui::MenuItem("Redo");
				ImGui::Separator();
				ImGui::MenuItem("Cut");
				ImGui::MenuItem("Copy");
				ImGui::MenuItem("Paste");
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("View"))
			{
				ImGui::MenuItem("Viewport");
				ImGui::MenuItem("Scene Hierarchy");
				ImGui::MenuItem("Property Inspector");
				ImGui::EndMenu();
			}

			const char* text = "Tourqe Advanced Game Engine - Level Editor";
			float windowWidth = ImGui::GetWindowWidth();
			ImVec2 textSize = ImGui::CalcTextSize(text);
			float textPosX = (windowWidth - textSize.x) * 0.5f;

			ImGui::SetCursorPosX(textPosX);
			ImGui::Text("%s", text);

			ImGui::SameLine(ImGui::GetWindowWidth() - 100.0f);

			if (ImGui::ImageButton("iconify", (ImTextureID)(void*)GetIcon("I_Line"), ImVec2(IconSize, IconSize))) {
				GetMainWindow().IconifyWindow();
			}
			ImGui::SameLine();
			
			static bool maximized = false;
			if (ImGui::ImageButton("maximaze", (ImTextureID)(void*)GetIcon("I_Cube"), ImVec2(IconSize, IconSize))) {
				GetMainWindow().RestoreWindow(maximized);
				maximized = !maximized;
			}
			ImGui::SameLine();

			if (ImGui::ImageButton("Close", (ImTextureID)(void*)GetIcon("I_Close"), ImVec2(IconSize, IconSize))) {
				GetApp().Close();
			}

			if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows))
			{
				isDraggingWindow = true;
				dragStartPos = ImGui::GetMousePos();
			}

			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsWindowHovered(ImGuiHoveredFlags_RootAndChildWindows))
			{
				GetMainWindow().RestoreWindow(maximized);
				maximized = !maximized;
			}

			if (isDraggingWindow)
			{
				if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
				{
					ImVec2 currentPos = ImGui::GetMousePos();
					ImVec2 delta = ImVec2(currentPos.x - dragStartPos.x, currentPos.y - dragStartPos.y);
					dragStartPos = currentPos;

					int x, y;
					GetMainWindow().GetWindowPos(&x, &y);
					GetMainWindow().SetWindowPos(x + (int)delta.x, y + (int)delta.y);
				}

				if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
				{
					isDraggingWindow = false;
				}
			}

			ImGui::EndMainMenuBar();
		}
		
		ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_MenuBar;
		float height = ImGui::GetFrameHeight();

		if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::ImageButton("wireframemode", (ImTextureID)(void*)GetIcon("I_WireFrame"), ImVec2(IconSize, IconSize))) {
					GetApp().GetRenderer()._RenderMode = TERenderMode::WIREFRAME;
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Wireframe Mode");
				}

				ImGui::SameLine();

				if (ImGui::ImageButton("fillmode", (ImTextureID)(void*)GetIcon("I_Fill"), ImVec2(IconSize, IconSize))) {
					GetApp().GetRenderer()._RenderMode = TERenderMode::FULL;
				}
				if (ImGui::IsItemHovered()) {
					ImGui::SetTooltip("Fill Mode");
				}
				ImGui::SameLine();

				if (ImGui::ImageButton("translate", (ImTextureID)(void*)GetIcon("I_GTranslate"), ImVec2(IconSize, IconSize))) {
					if (_GizmoTypeRef) *_GizmoTypeRef = ImGuizmo::TRANSLATE;
				}
				ImGui::SameLine();
				if (ImGui::ImageButton("rotate", (ImTextureID)(void*)GetIcon("I_GRotate"), ImVec2(IconSize, IconSize))) {
					if (_GizmoTypeRef) *_GizmoTypeRef = ImGuizmo::ROTATE;
				}
				ImGui::SameLine();
				if (ImGui::ImageButton("scale", (ImTextureID)(void*)GetIcon("I_GScale"), ImVec2(IconSize, IconSize))) {
					if (_GizmoTypeRef) *_GizmoTypeRef = ImGuizmo::SCALE;
				}

				float iconSize = IconSize;
				float spacing = ImGui::GetStyle().ItemSpacing.x;
				float totalWidth = iconSize * 3 + spacing * 2;
				float centerPosX = (ImGui::GetWindowWidth() - totalWidth) * 0.5f;

				ImGui::SetCursorPosX(centerPosX);

				//if (!_Playing) {
				//	if (ImGui::ImageButton("Play", (ImTextureID)(void*)GetIcon("I_Play"), ImVec2(iconSize, iconSize)))
				//	{
				//		_Playing = true;
				//		GetApp().SetEngineMode(TEEngineMode::GAME);
				//	}
				//	ImGui::SameLine();
				//}

				//if (_Playing) {
				//	if (ImGui::ImageButton("Pause", (ImTextureID)(void*)GetIcon("I_Pause"), ImVec2(iconSize, iconSize)))
				//	{
				//		GetApp().SetEngineMode(TEEngineMode::EDITOR);
				//		_Playing = false;
				//	}
				//	ImGui::SameLine();
				//}

				//if (ImGui::ImageButton("Stop", (ImTextureID)(void*)GetIcon("I_Stop"), ImVec2(iconSize, iconSize)))
				//{
				//	GetApp().SetEngineMode(TEEngineMode::EDITOR);
				//}

				ImGui::EndMenuBar();
			}
		}
		ImGui::End();
	}

	void SetGizmoTypeReference(ImGuizmo::OPERATION* ref) { _GizmoTypeRef = ref; }
	private:
		ImGuizmo::OPERATION* _GizmoTypeRef = nullptr;
		bool _Playing = false;
};