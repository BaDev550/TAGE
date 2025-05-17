#pragma once

#include "TAGE/ECS/World/World.h"
#include "TAGE/ECS/GSM/EventSystem.h"
#include "TAGE/ECS/GSM/ServiceLocator.h"

namespace TAGE::GAMEFACTORY {
	class GameInstance
	{
	public:
		GameInstance() = default;
		~GameInstance() = default;

		void Init();
		void Update(float dt);
		void Shutdown();

		World* GetCurrentWorld() const { return _CurrentWorld.get(); };

		void Reset() {
			_CurrentWorld.reset();
			Init();
		}

	private:
		MEM::Scope<World> _CurrentWorld = nullptr;

		float _FixedDeltaTime = 1.0f / 60.0f;
		float _Accumulator = 1.0f;
	};
}