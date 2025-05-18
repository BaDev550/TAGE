#include "tagepch.h"
#include "GameInstance.h"

namespace TAGE::GAMEFACTORY {
	GameInstance* GameInstance::s_Instance = nullptr;

	void GameInstance::Init()
	{
		s_Instance = this;

		CORE_LOG_INFO("GameInstance initializing...");
		auto eventSystem = MEM::CreateRef<EventSystem>();
		ServiceLocator::RegisterService<EventSystem>(eventSystem);

		_CurrentWorld = MEM::CreateRef<World>();
		_CurrentWorld->Init();
	}
	void GameInstance::Update(float dt)
	{
		if (_GameMode)
			_GameMode->Tick(dt);

		_CurrentWorld->Update(dt, SystemUpdateMode::GAME);
		_Accumulator += dt;
		while (_Accumulator >= _FixedDeltaTime) {
			_CurrentWorld->FixedUpdate(dt, SystemUpdateMode::GAME);
			_Accumulator -= _FixedDeltaTime;
		}
	}
	void GameInstance::Shutdown()
	{

	}
}