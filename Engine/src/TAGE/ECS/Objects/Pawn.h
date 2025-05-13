#pragma once

#include "GameObject.h"
#include "PawnCamera.h"

namespace TAGE::ECS {
	class Pawn : public GameObject
	{
	public:
		Pawn(const std::string& name) : GameObject(name, ObjectType::OBJECT_PAWN) {
			SkeletalMesh = &AddComponent<SkeletalMeshComponent>();
			Collider = &AddComponent<ColliderComponent>();
			Collider->Shape = ColliderShapeType::CAPSULE;
			Collider->Size.y = 1.5f;

			Application::Get().GetPhysicsSystem().CreateRigidBody(this, 1.0f);
			RigidBody = &GetComponent<RigidBodyComponent>();

			AddChild(&Camera);
		}

		virtual void Tick(float deltaTime) override {
			glm::vec2 mousePos = Input::GetMousePosition();
			glm::vec2 delta = (mousePos - _LastMousePosition) * 0.1f;
			_LastMousePosition = mousePos;

			auto& tc = GetComponent<TransformComponent>();

			Camera.AddYaw(delta.x);
			Camera.AddPitch(-delta.y);

			Camera.ProcessCamera(tc, deltaTime);
		}

	protected:
		SkeletalMeshComponent* SkeletalMesh;
		ColliderComponent* Collider;
		RigidBodyComponent* RigidBody;
		PawnCamera Camera;

	private:
		glm::vec2 _LastMousePosition = glm::vec2(0.0f);
	};
}