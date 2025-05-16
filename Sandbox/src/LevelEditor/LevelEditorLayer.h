#include "TAGE/TAGE.h"
#include "TAGE/TComponents.h"
#include "TAGE/TObjects.h"
#include "TAGE/TEvents.h"
#include "TAGE/TPhysics.h"
#include "Wall.h"

#include "imgui.h"
#include "ImGuizmo.h"

namespace LEVELEDITOR {
	class LevelEditorLayer : public TELayer {
	public:
		LevelEditorLayer() : TELayer("Level Editor Layer") {
			GetApp().SetEngineMode(TEEngineMode::EDITOR);
			TERenderer::_RenderMode = TERenderMode::VIEWPORT;

			wall = new Wall("Assets/Models/Character/Char.glb", glm::vec3(0.0f), glm::vec3(1.0f));
		}
		~LevelEditorLayer() {
			delete wall;
		}

		virtual void OnUpdate(float dt) override {
			camera.ProcessCamera(dt);
		}

		bool open = true;
		void OnImGuiRender() override {
			ImGui::Begin("Viewport", &open, ImGuiWindowFlags_NoMove);

			_ViewportPanelSize = ImGui::GetContentRegionAvail();
			_ViewportPanelPos = ImGui::GetCursorScreenPos();

			uint32_t id = GetMainFramebuffer()->GetColorAttachment();
			ImGui::Image((ImTextureID)(void*)id, ImVec2(1000, 500), ImVec2(0, 1), ImVec2(1, 0));

			DrawEntityGizmos(_SelectedObject);

			ImGui::End();
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

			if (event.GetKey() == TEKey::E)
				_GizmoType = ImGuizmo::TRANSLATE;
			if (event.GetKey() == TEKey::R)
				_GizmoType = ImGuizmo::ROTATE;
			if (event.GetKey() == TEKey::Q)
				_GizmoType = ImGuizmo::SCALE;

			return false;
		}

		TEHitResult RayFromMouse(glm::vec2 delta) {
			glm::mat4 view = camera.GetCamera().GetViewMatrix();
			glm::mat4 projection = camera.GetCamera().GetProjectionMatrix();

			glm::vec3 rayDir = CastHelper::ScreenToRay(delta, view, projection);
			glm::vec3 rayOrigin = camera.GetWorldLocation();
			glm::vec3 rayEnd = rayOrigin + rayDir * 10000.0f;

			return Cast::Raycast(nullptr, rayOrigin, rayEnd, false, TERayDrawType::FOR_DURATION, 1.0f);
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

		void DrawEntityGizmos(TGameObject* selectedObject) {
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

		bool OnMouseButtonPressed(TEMouseButtonPressedE& event) {
			if (event.GetButton() == TEMouseButton::Left) {
				glm::vec2 delta = TEInput::GetMousePosition();
				TEHitResult ray = RayFromMouse(delta);
				if (ray.hit) {
					_SelectedObject = ray.actor;
				}
				else {
					_SelectedObject = nullptr;
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
		TGameObject* _SelectedObject = nullptr;

		Wall* wall;
		bool _Cursor = false;

		ImVec2 _ViewportPanelSize, _ViewportPanelPos;
		ImGuizmo::OPERATION _GizmoType = ImGuizmo::TRANSLATE;
	};
}