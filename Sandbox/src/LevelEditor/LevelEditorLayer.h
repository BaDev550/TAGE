#include "TAGE/TAGE.h"
#include "TAGE/TComponents.h"
#include "TAGE/TObjects.h"
#include "TAGE/TEvents.h"
#include "TAGE/TPhysics.h"

#include "imgui.h"
#include "imgui_internal.h"
#include "ImGuizmo.h"

#include "TeGui.h"
#include "Panels/MainMenuBar.h"
#include "Panels/ScenePanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Panels/LogPanel.h"

namespace LEVELEDITOR {
	class LevelEditorLayer : public TELayer {
	public:
		LevelEditorLayer() : TELayer("Level Editor Layer") {
			GetApp().SetEngineMode(TEEngineMode::EDITOR);
			TERenderer::_RenderMode = TERenderMode::VIEWPORT;
			_MainMenuBar.SetGizmoTypeReference(&_GizmoType);
		}
		~LevelEditorLayer() {
		}

		virtual void OnImGuiRender() override {
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar |
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
			if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
			{
				ImGui::DockBuilderRemoveNode(dockspace_id);
				ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
				ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

				ImGuiID dock_main_id = dockspace_id;
				ImGuiID dock_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.2f, nullptr, &dock_main_id);
				ImGuiID dock_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.25f, nullptr, &dock_main_id);
				ImGuiID dock_bottom = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Down, 0.25f, nullptr, &dock_main_id);

				ImGui::DockBuilderDockWindow("Scene Hierarchy", dock_left);
				ImGui::DockBuilderDockWindow("Property Inspector", dock_right);
				ImGui::DockBuilderDockWindow("Asset Browser", dock_bottom);
				ImGui::DockBuilderDockWindow("Logs", dock_bottom);
				ImGui::DockBuilderDockWindow(GetApp().GetScene().GetName().c_str(), dock_main_id);

				ImGui::DockBuilderFinish(dockspace_id);
			}

			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f),
				ImGuiDockNodeFlags_PassthruCentralNode);

			ImGui::End();

			_ScenePanel.Begin();
			_MainMenuBar.Begin();
			_ContentPanel.Begin();
			_LogPanel.Begin();

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 });
			ImGui::Begin(GetApp().GetScene().GetName().c_str(), nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

			_ViewportPanelSize = ImGui::GetContentRegionAvail();
			_ViewportPanelPos = ImGui::GetCursorScreenPos();
			_ViewportFocused = ImGui::IsWindowFocused();
			_ViewportHovered = ImGui::IsWindowHovered();

			if (_ViewportPanelSize.x > 0 && _ViewportPanelSize.y > 0) {
				if (camera.IsValid())
					camera.GetCamera().GetCamera()->SetAspectRatio((float)_ViewportPanelSize.x / (float)_ViewportPanelSize.y);
				//GetMainFramebuffer()->Resize((uint32_t)_ViewportPanelSize.x, (uint32_t)_ViewportPanelSize.y);
			}

			uint32_t id = GetMainFramebuffer()->GetColorAttachment();
			ImGui::Image(
				(ImTextureID)(void*)(intptr_t)id,
				_ViewportPanelSize,
				ImVec2(0, 1), ImVec2(1, 0)
			);

			DrawEntityGizmos(_ScenePanel.GetSelectedActor());
			
			ImGui::End();
			ImGui::PopStyleVar();
		}

		virtual void OnUpdate(float dt) override {
			if (camera.IsValid())
				camera.ProcessCamera(dt);
		}

		bool OnKeyPressed(TEKeyPressedE& event) {
			if (event.GetKey() == TEKey::Escape) {
				GetApp().Close();
				return true;
			}
			if (event.GetKey() == TEKey::F1) {
				_Cursor = !_Cursor;
				GetMainWindow().EnableCursor(_Cursor);
			}

			if (event.GetKey() == TEKey::F2) {
				GetApp().GetSceneSerializer().Save("scene.tagescene");
			}
			if (event.GetKey() == TEKey::F3) {
				GetApp().GetSceneSerializer().Load("scene.tagescene");
			}
			if (event.GetKey() == TEKey::F4) {
				_SelectedObject = nullptr;
			}

			if (event.GetKey() == TEKey::E)
				_GizmoType = ImGuizmo::TRANSLATE;
			if (event.GetKey() == TEKey::R)
				_GizmoType = ImGuizmo::ROTATE;
			if (event.GetKey() == TEKey::Q)
				_GizmoType = ImGuizmo::SCALE;
			return false;
		}

		bool DecomposeMatrix(const glm::mat4& transform,
			glm::vec3& outTranslation,
			glm::quat& outRotation,
			glm::vec3& outScale)
		{
			glm::mat4 localMatrix = transform;

			if (glm::epsilonEqual(localMatrix[3][3], 0.0f, glm::epsilon<float>()))
				return false;

			outTranslation = glm::vec3(localMatrix[3]);
			localMatrix[3] = glm::vec4(0, 0, 0, localMatrix[3].w);

			outScale.x = glm::length(localMatrix[0]);
			localMatrix[0] = glm::normalize(localMatrix[0]);

			outScale.y = glm::length(localMatrix[1]);
			localMatrix[1] = glm::normalize(localMatrix[1]);

			outScale.z = glm::length(localMatrix[2]);
			localMatrix[2] = glm::normalize(localMatrix[2]);

			glm::mat3 rotationMatrix(localMatrix);

			outRotation = glm::quat_cast(rotationMatrix);

			return true;
		}

		void DrawEntityGizmos(TActor* selectedObject) {
			if (!selectedObject) return;

			auto& transform = selectedObject->GetComponent<TTransformComponent>();

			glm::mat4 modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(modelMatrix, transform.Position);
			modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.Rotation.x), glm::vec3(1, 0, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.Rotation.y), glm::vec3(0, 1, 0));
			modelMatrix = glm::rotate(modelMatrix, glm::radians(transform.Rotation.z), glm::vec3(0, 0, 1));
			modelMatrix = glm::scale(modelMatrix, transform.Scale);

			ImGuizmo::SetOrthographic(false);
			ImGuizmo::BeginFrame();
			ImGuizmo::SetDrawlist();

			ImGuizmo::SetRect(_ViewportPanelPos.x, _ViewportPanelPos.y, _ViewportPanelSize.x, _ViewportPanelSize.y);
			glm::mat4 viewMatrix = camera.GetCamera().GetViewMatrix();
			glm::mat4 projectionMatrix = camera.GetCamera().GetProjectionMatrix();

			ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
				_GizmoType, ImGuizmo::WORLD, glm::value_ptr(modelMatrix));

			_UsingGizmo = ImGuizmo::IsOver();
			if (ImGuizmo::IsUsing()) {
				glm::vec3 newPosition;
				glm::vec3 newScale;
				glm::quat rotationQuat;

				if (DecomposeMatrix(modelMatrix, newPosition, rotationQuat, newScale)) {
					transform.Position = newPosition;
					transform.Rotation = glm::degrees(glm::eulerAngles(rotationQuat));
					transform.Scale = newScale;
				}
			}
		}

		TEHitResult RayFromMouse(glm::vec2 delta) {
			glm::mat4 view = camera.GetCamera().GetViewMatrix();
			glm::mat4 projection = camera.GetCamera().GetProjectionMatrix();

			glm::vec3 rayDir = CastHelper::ScreenToRay(delta, view, projection);
			glm::vec3 rayOrigin = camera.GetWorldLocation();
			glm::vec3 rayEnd = rayOrigin + rayDir * 10000.0f;

			return Cast::Raycast(nullptr, rayOrigin, rayEnd, false, TERayDrawType::FOR_DURATION, 1.0f);
		}

		bool OnMouseButtonPressed(TEMouseButtonPressedE& event) {
			if (event.GetButton() == TEMouseButton::Left && (_ViewportHovered && !_UsingGizmo)) {
				glm::vec2 delta = TEInput::GetMousePosition();
				TEHitResult ray = RayFromMouse(delta);
				if (ray.hit) {
					_ScenePanel.SetSelectedActor(ray.actor);
				}
				else {
					_ScenePanel.SetSelectedActor(nullptr);
				}
			}
			return false;
		}

		void OnEvent(TEEvent& event) override {
			TEEventDistpacher dispatcher(event);
			dispatcher.Dispatch<TEKeyPressedE>(BIND_EVENT_FN(LevelEditorLayer::OnKeyPressed));
			dispatcher.Dispatch<TEMouseButtonPressedE>(BIND_EVENT_FN(LevelEditorLayer::OnMouseButtonPressed));
		}

	private:
		TEditorCamera camera;
		TActor* _SelectedObject = nullptr;

		MainMenuBar _MainMenuBar;
		ScenePanel _ScenePanel;
		ContentBrowserPanel _ContentPanel;
		LogPanel _LogPanel;

		ImVec2 _ViewportPanelSize, _ViewportPanelPos;
		bool _ViewportFocused = false, _ViewportHovered = false;
		bool _UsingGizmo = false;
		ImGuizmo::OPERATION _GizmoType = ImGuizmo::TRANSLATE;

		bool _Cursor = false;
	};
}