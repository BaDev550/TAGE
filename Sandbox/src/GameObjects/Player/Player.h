#pragma once

#include "TAGE/TAGE.h"
#include "TAGE/TObjects.h"
#include "TAGE/TPhysics.h"
#include "TAGE/TMeshes.h"
#include "imgui.h"

class Player : public TCharacter
{
public:
	Player() : TCharacter("Player") {
		SkeletalMesh->LoadModel("Assets/Models/Arms/Arms.glb");

		auto& tc = GetComponent<TTransformComponent>();
		tc.LocalPosition.y += 1.36f;

		auto& ctc = Camera.GetComponent<TTransformComponent>();
		ctc.LocalPosition.y += 1.4f;
		ctc.LocalRotation.y += 90.0f;
		Camera.RotationLag(true);
		Camera.SetCameraLagRotationSpeed(15.0f);
		
		idleAnim = new TEAnim("Assets/Models/Arms/Idle.glb", SkeletalMesh->model.get());
		walkAnim = new TEAnim("Assets/Models/Arms/Walk_01.glb", SkeletalMesh->model.get());
		Animator = AddComponent<TAnimatorComponent>(SkeletalMesh->GetSkeleton(), idleAnim);
	}
	~Player() {
		delete idleAnim;
		delete walkAnim;
	}

	void Fire() {
		auto& transform = GetComponent<TTransformComponent>();
		auto& rb = GetComponent<TRigidBodyComponent>();
		glm::vec3 start = GetWorldLocation();
		glm::vec3 end = start + Camera.GetForward() * 500.0f;

		TEHitResult hit = Cast::Raycast(this, start, end, true, TERayDrawType::FOR_DURATION, 1.0f);

		if (hit.hit) {
			std::cout << hit.actor->GetComponent<TTagComponent>().tag << std::endl;
		}
	}

	virtual void Tick(float deltaTime) override {
		TCharacter::Tick(deltaTime);
		if (glm::length(RigidBody->GetVelocity()) > 10.0f) {
			if (Animator.AnimatorInstance->GetCurrentAnimation() != walkAnim)
				Animator.AnimatorInstance->PlayAnimation(walkAnim);
		}
		else {
			if (Animator.AnimatorInstance->GetCurrentAnimation() != idleAnim)
				Animator.AnimatorInstance->PlayAnimation(idleAnim);
		}
	};
private:
	TEAnim* idleAnim;
	TEAnim* walkAnim;
	TAnimatorComponent Animator;
};

