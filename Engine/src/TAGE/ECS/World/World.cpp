#include "tagepch.h"
#include "World.h"

namespace TAGE {
	World::~World()
	{
	}

	void World::Init()
	{
		if (_CurrentScene)
			_CurrentScene->Init();
		else
			CORE_LOG_WARN("No scene set for World!");
	}

	void World::Update(float dt, SystemUpdateMode mode)
	{
		if (_CurrentScene)
			_CurrentScene->UpdateSystems(dt, mode);
	}

	void World::FixedUpdate(float dt, SystemUpdateMode mode)
	{
		if (_CurrentScene)
			_CurrentScene->FixedUpdateSystems(dt, mode);
	}
}