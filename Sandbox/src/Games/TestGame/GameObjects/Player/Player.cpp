#include "Player.h"

void Player::Tick(float deltaTime) {
	TCharacter::Tick(deltaTime);
	AnimInstance->SetParameter("CameraForward", Camera.GetForward());
	AnimInstance->SetParameter("PlayerSpeed", glm::length(RigidBody->GetVelocity()));

	TESocket* socket = SkeletalMesh->GetSkeleton()->GetSocket("Weapon");
	if (socket) {
	}

	if (TEInput::IsKeyPressed(TEKey::E)) {
		TEHitResult interaction_result = Interact();
		if (interaction_result.hit) {
			if (interaction_result.actor->HasTag("Interactable")) {
				if (dynamic_cast<IInteractable*>(interaction_result.actor)) {
					auto* IF = dynamic_cast<IInteractable*>(interaction_result.actor);
					IF->Interact();
				}
			}
		}
	}
}
