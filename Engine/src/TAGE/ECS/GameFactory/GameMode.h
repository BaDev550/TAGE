#pragma once

#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/ECS/Objects/Pawn.h"
#include "TAGE/ECS/World/World.h"
#include "GameState.h"
#include "PlayerController.h"

namespace TAGE {
	class GameMode {
	public:
		virtual ~GameMode() = default;

		virtual void Init(World* world);
		virtual void OnPlayerJoin(Entity playerEntity);

		virtual MEM::Ref<PlayerController> CreatePlayerController(Entity playerEntity);
		virtual MEM::Ref<Pawn> CreateDefaultPawn(Entity playerEntity);

	protected:
		World* _World = nullptr;
	};
}