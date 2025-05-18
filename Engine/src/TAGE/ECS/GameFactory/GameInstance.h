#pragma once

#include "TAGE/ECS/World/World.h"
#include "TAGE/ECS/GameFactory/GameMode.h"
#include "TAGE/ECS/GSM/EventSystem.h"
#include "TAGE/ECS/GSM/ServiceLocator.h"
#include "TAGE/ECS/GameFactory/CameraManager.h"

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
		CameraManager& GetCameraManager() { return _CameraManager; }

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

		static GameInstance* Get() { return s_Instance; }
	private:
		MEM::Ref<World> _CurrentWorld = nullptr;
		MEM::Ref<GameMode> _GameMode;
		CameraManager _CameraManager;

		float _FixedDeltaTime = 1.0f / 60.0f;
		float _Accumulator = 1.0f;

		static GameInstance* s_Instance;
	};
}