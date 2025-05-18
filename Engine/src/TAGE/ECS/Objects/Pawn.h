#pragma once

#include "TAGE/ECS/World/World.h"
#include "TAGE/ECS/Objects/Entity.h"

namespace TAGE {
	class Pawn : public Entity {
	public:
		Pawn(entt::entity handle, entt::registry* registry)
			: Entity(handle, registry) {
		}
		virtual ~Pawn() = default;

		virtual void Tick(float dt) {}
	};
}