#pragma once

#include "TAGE/TAGE.h"
#include "TAGE/TObjects.h"
#include "TAGE/TPhysics.h"
#include "TAGE/TMeshes.h"
#include "../../Interfaces/Interaction.h"

#include "imgui.h"

class Player : public TCharacter
{
public:
	Player() : TCharacter("Player") {
		SkeletalMesh->LoadModel("Assets/Models/Arms/Arms.glb");

		GetLocalLocation().y += 1.36f;

		auto& ctc = Camera.GetComponent<TTransformComponent>();
		ctc.LocalPosition.y += 1.4f;
		ctc.LocalRotation.y += 90.0f;
		
		idleAnim = new TEAnim("Assets/Models/Arms/Idle.glb", SkeletalMesh->GetModel());
		idleAnim->SetLoop(true);

		walkAnim = new TEAnim("Assets/Models/Arms/Idle.glb", SkeletalMesh->GetModel());
		walkAnim->SetLoop(true);

		Animator = AddComponent<TAnimatorComponent>(SkeletalMesh->GetSkeleton(), idleAnim);
	}
	~Player() {
		delete idleAnim;
		delete walkAnim;
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
	TEAnim* idleAnim;
	TEAnim* walkAnim;
	TAnimatorComponent Animator;
};

