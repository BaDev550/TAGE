#include "tagepch.h"
#include "GameInstance.h"

namespace TAGE::GAMEFACTORY {
	void GameInstance::Init()
	{
		CORE_LOG_INFO("GameInstance initializing...");
		auto eventSystem = MEM::CreateRef<EventSystem>();
		ServiceLocator::RegisterService<EventSystem>(eventSystem);

		_CurrentWorld = MEM::CreateScope<World>();
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