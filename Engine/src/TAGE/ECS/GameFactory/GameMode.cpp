#include "tagepch.h"
#include "GameMode.h"

namespace TAGE {
	void GameMode::Init(World* world)
	{
		_World = world;
		_GameState = MEM::CreateRef<GameState>();
		_GameState->Init(world);
	}

	void GameMode::Tick(float dt)
	{
		if (_GameState)
			_GameState->Tick(dt);
	}
}