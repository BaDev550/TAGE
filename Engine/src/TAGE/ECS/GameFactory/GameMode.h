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
		virtual void Tick(float dt);

		virtual void OnPlayerJoined(MEM::Ref<PlayerController> controller) {}
		virtual void OnPlayerLeave(MEM::Ref<PlayerController> controller) {}

		MEM::Ref<GameState> GetGameState() const { return _GameState; }

	protected:
		MEM::Ref<GameState> _GameState;
		World* _World = nullptr;
	};
}