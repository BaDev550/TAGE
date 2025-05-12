#pragma once

#include "TAGE/TAGE.h"

class PlayerCamera : public TGameObject
{
public:
	PlayerCamera() : TGameObject("Player Camera", TObjectType::OBJECT_CAMERA) {
		//AddComponent<TStaticMeshComponent>("Assets/Models/Cube/cube.obj");
		Component = &AddComponent<TCameraComponent>(TCamType::Perfective);
	}

	inline TCameraComponent& GetCamera() const { return *Component; }

	void ProcessCamera(TTransformComponent& transform) {
		glm::vec2 mousePos = TEInput::GetMousePosition();
		mousePos.y = -TEInput::GetMousePosition().y;
		glm::vec2 delta = (mousePos - _LastMousePosition) * 0.1f;
		_LastMousePosition = mousePos;
		float sensitivity = 1.0f;

		_yaw += delta.x * sensitivity;
		_pitch += delta.y * sensitivity;
		_pitch = glm::clamp(_pitch, -89.0f, 89.0f);

		transform.Rotation = -glm::vec3(0.0f, _yaw, 0.0f);
		Component->Camera->SetRotation(glm::vec3(_pitch, _yaw, 0.0f));
		Component->Camera->SetPosition(transform.Position + glm::vec3(0, 1.7f, 0));
	}

	glm::vec3 GetForward() { return Component->Camera->GetFront(); }
private:
	TCameraComponent* Component;
	float _pitch = 0.0f;
	float _yaw = 0.0f;
	glm::vec2 _LastMousePosition = glm::vec2(0.0f);
};