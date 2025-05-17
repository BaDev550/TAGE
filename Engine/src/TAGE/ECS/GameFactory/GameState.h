#pragma once

namespace TAGE {
	class GameState {
	public:
		GameState() = default;
		virtual ~GameState() = default;

		virtual void Init() {}
		virtual void Tick(float dt) {}
	};
}
