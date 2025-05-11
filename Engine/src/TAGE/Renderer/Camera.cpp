#include "tagepch.h"
#include "Camera.h"

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
        glm::vec3 right = glm::normalize(glm::cross(direction, glm::vec3(0, 1, 0)));
        glm::vec3 up = glm::cross(right, direction);

        _ViewMatrix = glm::lookAt(_Position, _Position + direction, up);
        _ProjectionMatrix = glm::perspective(glm::radians(_FOV), _AspectRatio, _NearClip, _FarClip);
    }

	void PerspectiveCamera::OnUpdate(float deltaTime) {
        glm::vec2 mousePos = Input::GetMousePosition();
        mousePos.y = -Input::GetMousePosition().y;
        glm::vec2 delta = (mousePos - _LastMousePosition) * 0.1f;
        _LastMousePosition = mousePos;

        _Rotation.y += delta.x;
        _Rotation.x += delta.y;
        _Rotation.x = glm::clamp(_Rotation.x, -89.0f, 89.0f);

        UpdateViewMatrix();
	}

	void EditorCamera::OnUpdate(float deltaTime) {
        glm::vec2 mousePos = Input::GetMousePosition();
        mousePos.y = -Input::GetMousePosition().y;
        glm::vec2 delta = (mousePos - _LastMousePosition) * 0.1f;
        _LastMousePosition = mousePos;

        if (Input::IsMouseButtonPressed(MouseButton::Right)) {
            _Rotation.y += delta.x;
            _Rotation.x += delta.y;
            _Rotation.x = glm::clamp(_Rotation.x, -89.0f, 89.0f);
        }

        glm::vec3 right = glm::normalize(glm::cross(_Forward, glm::vec3(0, 1, 0)));

        if (Input::IsKeyPressed(KeyCode::W)) _Position += _Forward * _MovementSpeed * deltaTime;
        if (Input::IsKeyPressed(KeyCode::S)) _Position -= _Forward * _MovementSpeed * deltaTime;
        if (Input::IsKeyPressed(KeyCode::A)) _Position -= right * _MovementSpeed * deltaTime;
        if (Input::IsKeyPressed(KeyCode::D)) _Position += right * _MovementSpeed * deltaTime;

        UpdateViewMatrix();
	}
}
