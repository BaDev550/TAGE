#include "tagepch.h"
#include "Camera.h"
#include "TAGE/Core/Application/Application.h"

namespace TAGE::RENDERER {

	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: _Position(0.0f), _Rotation(0.0f), _FOV(fov), _AspectRatio(aspectRatio),
		_NearClip(nearClip), _FarClip(farClip) {
		UpdateViewMatrix();
	}

	void PerspectiveCamera::SetPosition(const glm::vec3& position) {
		_Position = position;
		UpdateViewMatrix();
	}

	void PerspectiveCamera::SetRotation(const glm::vec3& rotation) {
		_Rotation = rotation;
		UpdateViewMatrix();
	}

    void PerspectiveCamera::UpdateViewMatrix()
    {
        _Forward.x = cos(glm::radians(_Rotation.y)) * cos(glm::radians(_Rotation.x));
        _Forward.y = sin(glm::radians(_Rotation.x));
        _Forward.z = sin(glm::radians(_Rotation.y)) * cos(glm::radians(_Rotation.x));

        glm::vec3 direction = glm::normalize(_Forward);
        _Right = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::cross(_Right, direction);

        _ViewMatrix = glm::lookAt(_Position, _Position + direction, up);
        _ProjectionMatrix = glm::perspective(glm::radians(_FOV), _AspectRatio, _NearClip, _FarClip);
    }


	void EditorCamera::Update() {
		if (Input::IsMouseButtonPressed(MouseButton::Right))
		{
			glm::vec2 mousePos = Input::GetMousePosition();
			mousePos.y = -mousePos.y;
			glm::vec2 delta = (mousePos - _LastMousePosition) * 1.0f;
			_LastMousePosition = mousePos;

			_Rotation.x += delta.y * _RotationSpeed;
			_Rotation.y += delta.x * _RotationSpeed;

			_Rotation.x = glm::clamp(_Rotation.x, -89.0f, 89.0f);
		}

		glm::vec3 forward = GetFront();
		forward.y = 0.0f;
		forward = glm::normalize(forward);
		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

		glm::vec3 moveDirection(0.0f);

		if (Input::IsKeyPressed(KeyCode::W)) moveDirection += forward;
		if (Input::IsKeyPressed(KeyCode::S)) moveDirection -= forward;
		if (Input::IsKeyPressed(KeyCode::A)) moveDirection -= right;
		if (Input::IsKeyPressed(KeyCode::D)) moveDirection += right;

		_Position += moveDirection * _MovementSpeed * (float)TAGE::Application::Get().GetDeltaTime();

		UpdateViewMatrix();
	}
}
