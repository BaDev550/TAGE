#pragma once

#include "TAGE/ECS/Objects/Actor.h"
#include "TAGE/Core/Utilities/RaycastUtils.h"
#include "TAGE/Physics/Raycaster.h"
#include "TAGE/ECS/ECS/Components/PhysicsComponents.h"
#include "TObjects.h"
#include "TComponents.h"

using TEHitResult = TAGE::PHYSICS::RAYCAST::RaycastHit;
using TERayDrawType = TAGE::PHYSICS::RAYCAST::RayDrawType;
using TECast = TAGE::PHYSICS::RAYCAST::Raycaster;

namespace Cast {
	inline static TEHitResult Raycast(TAGE::ECS::Actor* caller, const glm::vec3& from, const glm::vec3& to, bool ignoreSelf = true, TERayDrawType draw = TERayDrawType::FOR_FRAME, float draw_time = 0.0f) { TECast::CurrentCaller = caller; TEHitResult res = TECast::Raycast(from, to, ignoreSelf, draw, draw_time); TECast::CurrentCaller = nullptr; return res; }
	inline void CreateRigidBodyWithCollider(TActor* actor, float mass = 1.0f) { actor->AddComponent<TColliderComponent>(); GetWorld().GetPhysicsSystem().CreateRigidBody(actor, mass); }
}
