#pragma once

#include "TAGE/ECS/World/World.h"
#include "TAGE/ECS/Objects/Pawn.h"

namespace TAGE {
	class PlayerController {
	public:
		PlayerController(Entity owner) : _Owner(owner) {}
		virtual ~PlayerController() = default;

		virtual void SetupInput() = 0;
		virtual void Tick(float dt) = 0;

		void Possess(MEM::Ref<Pawn> pawn);
		Entity GetOwnerEntity() const { return _Owner; }

	protected:
		Entity _Owner;
		MEM::Ref<Pawn> _ControlledPawn;
	};
}