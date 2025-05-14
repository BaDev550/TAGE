#include "Player.h"

void Player::Tick(float deltaTime) {
	TCharacter::Tick(deltaTime);
	if (glm::length(RigidBody->GetVelocity()) > 10.0f) {
		if (Animator.GetInstance()->GetCurrentAnimation() != walkAnim)
			Animator.GetInstance()->PlayAnimationWithBlend(walkAnim, 0.3f);
	}
	else {
		if (Animator.GetInstance()->GetCurrentAnimation() != idleAnim)
			Animator.GetInstance()->PlayAnimationWithBlend(idleAnim, 0.3f);
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
