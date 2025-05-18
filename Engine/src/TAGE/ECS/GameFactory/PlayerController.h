#pragma once

#include "TAGE/ECS/World/World.h"
#include "TAGE/ECS/Objects/Pawn.h"
#include "TAGE/ECS/Components/InputComponent.h"

namespace TAGE {
	class PlayerController {
	public:
		PlayerController() = default;
		virtual ~PlayerController() = default;

		virtual void Init(World* world);
		virtual void Tick(float dt);

		void Possess(Entity* pawn) { _Pawn = pawn; }
		Entity* GetPawn() const { return _Pawn; }

	protected:
		Entity* _Pawn = nullptr;
		World* _World = nullptr;
		MEM::Ref<ECS::InputComponent> _InputComponent;
	};
}