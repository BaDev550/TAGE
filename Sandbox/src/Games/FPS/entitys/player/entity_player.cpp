#include "entity_player.h"

void entity_player::Tick(float deltaTime) {
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

			}
		}
	}
}
