#pragma once

#include "TAGE/TAGE.h"
#include "TAGE/TObjects.h"
#include "TAGE/TPhysics.h"
#include "TAGE/TMeshes.h"
#include "PlayerAnimInstance.h"
#include "../../Interfaces/Interaction.h"

#include "imgui.h"

class Player : public TCharacter
{
public:
	Player() : TCharacter("Player") {
		SkeletalMesh->LoadModel("Assets/Models/Character/Char.glb");
		AnimInstance = SkeletalMesh->GetSkeleton()->SetAnimInstance<PlayerAnimInstance>(this, SkeletalMesh->GetModel());
		AnimInstance->LoadAnimation("Idle", "Assets/Models/Arms/Idle.glb");
		//AnimInstance->LoadAnimation("Walk", "Assets/Models/Arms/Walk.glb");
		AnimInstance->Play("Idle");

		_bUseControllYawRotation = true;
		GetLocalRotation().x += 90.0f;

		auto& ctc = Camera.GetComponent<TTransformComponent>();
		ctc.LocalPosition.y += 1.4f;
		ctc.LocalRotation.y += 90.0f;
		Camera.RotationLag(true);
		Camera.SetCameraLagRotationSpeed(15.0f);

		TESocket socket("Weapon", 0);
		SkeletalMesh->GetSkeleton()->AddSocketToBone("mixamorig1:RightHand", socket);
	}
	~Player() {
	}
	
	TEHitResult Interact() {
		auto& rb = GetComponent<TRigidBodyComponent>();
		glm::vec3 start = Camera.GetWorldLocation();
		glm::vec3 end = start + Camera.GetForward() * 150.0f;

		TEHitResult hit = Cast::Raycast(this, start, end, true, TERayDrawType::FOR_DURATION, 1.0f);

		return hit;
	}

	virtual void Tick(float deltaTime) override;;
private:
	TAGE::MEM::Ref<PlayerAnimInstance> AnimInstance;
};

REGISTER_GAMEOBJECT_CLASS(Player);