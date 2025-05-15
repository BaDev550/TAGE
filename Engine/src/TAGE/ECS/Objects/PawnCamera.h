#pragma once

#include "GameObject.h"
#include "TAGE/Core/Defines.h"

namespace TAGE::ECS {
	class PawnCamera : public GameObject
	{
	public:
		PawnCamera() : GameObject("Pawn Camera", ObjectType::OBJECT_CAMERA), _pitch(0.0f), _yaw(-90.0f), _Sens(1.0f) {
			Component = &AddComponent<CameraComponent>(ECameraType::PERSFECTIVE);
		}

		inline CameraComponent& GetCamera() const { return *Component; }
		glm::vec3 GetForward() { return Component->GetForward(); }

		void AddYaw(float deltaX) {
			_yaw += deltaX * _Sens;
		}

		void AddPitch(float deltaY) {
			_pitch += deltaY * _Sens;
			_pitch = glm::clamp(_pitch, -89.0f, 89.0f);
		}

		void ProcessCamera(TransformComponent& transform, float deltaTime) {
			glm::vec3 desiredPos = transform.Position + _targetOffset;
			_currentPos = glm::mix(_currentPos, desiredPos, _cameraLagSpeed * deltaTime);

			glm::vec2 targetRot = glm::vec2(_pitch, _yaw);
			_currentRot = glm::mix(_currentRot, targetRot, _rotationLagSpeed * deltaTime);

			transform.Rotation = -glm::vec3(_pitch, _yaw, 0.0f);
			Component->GetCamera()->SetRotation((_cameraLagRotationEnabled ? glm::vec3(_currentRot.x, _currentRot.y, 0.0f) : glm::vec3(_pitch, _yaw, 0.0f)) + GetComponent<TransformComponent>().LocalRotation);
			Component->GetCamera()->SetPosition(_cameraLagPositionEnabled ? _currentPos : desiredPos + GetComponent<TransformComponent>().LocalPosition);
		}

		void SetSensitivity(float sens) { _Sens = sens; }
		float GetSensitivity() const { return _Sens; }

		void SetCameraLagSpeed(float speed) { _cameraLagSpeed = speed; }
		float GetCameraLagSpeed() const { return _cameraLagSpeed; }

		void SetCameraLagRotationSpeed(float speed) { _rotationLagSpeed = speed; }
		float GetCameraLagRotationSpeed() const { return _rotationLagSpeed; }

		void SetTargetOffset(const glm::vec3& offset) { _targetOffset = offset; }
		const glm::vec3& GetTargetOffset() const { return _targetOffset; }

		void RotationLag(bool enabled) { _cameraLagRotationEnabled = enabled; }
		void PositionLag(bool enabled) { _cameraLagPositionEnabled = enabled; }
	private:
		CameraComponent* Component;
		float _pitch = 0.0f;
		float _yaw = -90.0f;
		float _Sens = 1.0f;

		glm::vec3 _currentPos = glm::vec3(0.0f);
		glm::vec2 _currentRot = glm::vec2(0.0f);
		glm::vec3 _targetOffset = glm::vec3(0, 0, 0);
		bool _cameraLagPositionEnabled = false;
		bool _cameraLagRotationEnabled = false;
		float _cameraLagSpeed = 5.0f;
		float _rotationLagSpeed = 8.0f;
	};

	REGISTER_GAMEOBJECT_CLASS(PawnCamera);
}