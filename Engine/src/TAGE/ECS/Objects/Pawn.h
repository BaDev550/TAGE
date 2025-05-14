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
		}

	protected:
		SkeletalMeshComponent* SkeletalMesh;
		ColliderComponent* Collider;
		RigidBodyComponent* RigidBody;
	};
}