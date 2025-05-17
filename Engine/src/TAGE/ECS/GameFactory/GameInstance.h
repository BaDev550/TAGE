#pragma once

#include "TAGE/ECS/World/World.h"
#include "TAGE/ECS/GameFactory/GameMode.h"
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

		template<typename T>
		void SetGameMode() {
			static_assert(std::is_base_of<GameMode, T>::value, "T must be derived from GameMode");
			_GameMode = MEM::CreateRef<T>();
			_GameMode->Init(_CurrentWorld.get());
		}

		void Reset() {
			_CurrentWorld.reset();
			Init();
		}

	private:
		MEM::Scope<World> _CurrentWorld = nullptr;
		MEM::Ref<GameMode> _GameMode;

		float _FixedDeltaTime = 1.0f / 60.0f;
		float _Accumulator = 1.0f;
	};
}