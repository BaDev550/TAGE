#pragma once

#include "GameObject.h"
#include "TAGE/Core/Defines.h"
#include "TAGE/ECS/ECS/Components/RenderingComponents.h"

namespace TAGE::ECS {
	class EditorCamera : public GameObject
	{
	public:
		EditorCamera() : GameObject("Editor Camera", ObjectType::OBJECT_CAMERA), _Sens(1.0f) {
			Component = &AddComponent<CameraComponent>(ECameraType::EDITOR);
		}

		inline CameraComponent& GetCamera() const { return *Component; }
		glm::vec3 GetForward() { return Component->GetForward(); }

        void ProcessCamera(float deltaTime) {
            glm::vec2 mousePos = Input::GetMousePosition();
            mousePos.y = -mousePos.y;
            glm::vec2 delta = (mousePos - _LastMousePosition) * 0.1f;
            _LastMousePosition = mousePos;

            if (Input::IsMouseButtonPressed(MouseButton::Right)) {
                _Rotation.y += delta.x;
                _Rotation.x += delta.y;
                _Rotation.x = glm::clamp(_Rotation.x, -89.0f, 89.0f);
            }

            _Forward.x = cos(glm::radians(_Rotation.y)) * cos(glm::radians(_Rotation.x));
            _Forward.y = sin(glm::radians(_Rotation.x));
            _Forward.z = sin(glm::radians(_Rotation.y)) * cos(glm::radians(_Rotation.x));
            _Forward = glm::normalize(_Forward);

            glm::vec3 right = glm::normalize(glm::cross(_Forward, glm::vec3(0, 1, 0)));

            if (Input::IsKeyPressed(KeyCode::W)) _Position += _Forward * _MovementSpeed * deltaTime;
            if (Input::IsKeyPressed(KeyCode::S)) _Position -= _Forward * _MovementSpeed * deltaTime;
            if (Input::IsKeyPressed(KeyCode::A)) _Position -= right * _MovementSpeed * deltaTime;
            if (Input::IsKeyPressed(KeyCode::D)) _Position += right * _MovementSpeed * deltaTime;

            Component->GetCamera()->SetRotation(_Rotation);
            Component->GetCamera()->SetPosition(_Position);
        }

		void SetSensitivity(float sens) { _Sens = sens; }
		float GetSensitivity() const { return _Sens; }
	private:
		CameraComponent* Component;
		float _Sens = 1.0f;
		glm::vec3 _Position = glm::vec3(0.0f);
		glm::vec3 _Rotation = glm::vec3(0.0f);
		glm::vec3 _Forward = glm::vec3(0.0f);
		glm::vec2 _LastMousePosition = glm::vec2(0.0f);
		float _MovementSpeed = 5.0f;
	};

	REGISTER_GAMEOBJECT_CLASS(EditorCamera);
}