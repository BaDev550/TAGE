#pragma once

#include "TAGE/ECS/World/World.h"

namespace TAGE {
	class Pawn {
	public:
		Pawn(Entity e) : _Entity(e) {}
		virtual ~Pawn() = default;

		virtual void Tick(float dt) = 0;

		Entity GetEntity() const { return _Entity; }

	protected:
		Entity _Entity;
	};
}