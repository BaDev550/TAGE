#pragma once

#include "PlayerController.h"

namespace TAGE {
	class GameState {
	public:
		GameState() = default;
		virtual ~GameState() = default;

		virtual void Init(World* world);
		virtual void Tick(float dt);

		const std::vector<MEM::Ref<PlayerController>>& GetPlayers() const { return _Players; }
		void AddPlayer(MEM::Ref<PlayerController> player) { _Players.push_back(player); }

	protected:
		std::vector<MEM::Ref<PlayerController>> _Players;
		World* _World = nullptr;
	};
}
