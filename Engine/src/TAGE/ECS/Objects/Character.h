#pragma once

#include "Pawn.h"
#include "TAGE/Physics/Raycaster.h"

namespace TAGE::ECS {
	class Character : public Pawn
	{
	public:
		Character(const std::string& name) : Pawn(name) {
			AddChild(&Camera);
		}

		bool IsGrounded() {
			auto& transform = GetComponent<TransformComponent>();
			glm::vec3 start = transform.Position;
			glm::vec3 end = start - glm::vec3(0, 1.3f, 0);

			PHYSICS::RAYCAST::Raycaster::CurrentCaller = this;
			PHYSICS::RAYCAST::RaycastHit hit = PHYSICS::RAYCAST::Raycaster::Raycast(start, end, true);
			PHYSICS::RAYCAST::Raycaster::CurrentCaller = nullptr;
			return hit.hit;
		}

		virtual void Tick(float deltaTime) override {
			Pawn::Tick(deltaTime);

			glm::vec2 mousePos = Input::GetMousePosition();
			glm::vec2 delta = (mousePos - _LastMousePosition) * 0.1f;
			_LastMousePosition = mousePos;

			auto& tc = GetComponent<TransformComponent>();

			Camera.AddYaw(delta.x);
			Camera.AddPitch(-delta.y);

			Camera.ProcessCamera(tc, deltaTime);

			glm::vec3 forward = Camera.GetForward();
			forward.y = 0.0f;
			forward = glm::normalize(forward);
			glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

			glm::vec3 moveDirection(0.0f);

			if (Input::IsKeyPressed(KeyCode::W)) moveDirection += forward;
			if (Input::IsKeyPressed(KeyCode::S)) moveDirection -= forward;
			if (Input::IsKeyPressed(KeyCode::A)) moveDirection -= right;
			if (Input::IsKeyPressed(KeyCode::D)) moveDirection += right;

			if (glm::length(moveDirection) > 0.1f)
				moveDirection = glm::normalize(moveDirection) * _MoveSpeed * deltaTime;

			RigidBody->SetVelocity(glm::vec3(moveDirection.x, RigidBody->GetVelocity().y, moveDirection.z));

			if (Input::IsKeyPressed(KeyCode::Space) && IsGrounded()) {
				RigidBody->SetVelocity(glm::vec3(RigidBody->GetVelocity().x, _JumpForce, RigidBody->GetVelocity().z));
			}
		}

	protected:
		float _MoveSpeed = 1700.0f;
		float _JumpForce = 5.0f;

		PawnCamera Camera;
		glm::vec2 _LastMousePosition = glm::vec2(0.0f);
	};
}