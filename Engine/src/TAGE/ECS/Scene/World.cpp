#include "tagepch.h"
#include "World.h"

namespace TAGE::ECS {
	World::~World()
	{
		for (auto actor : _Actors) {
			delete actor;
		}
	}
	void World::Init()
	{
		_entityRegistry = MEM::CreateScope<EntityRegistry>();
	}

	void World::Update(float dt, SystemUpdateMode mode)
	{
		for (auto& system : _systems)
			system->Update(_entityRegistry->GetRegistry(), dt, mode);
	}

	Actor* World::SpawnActor(const std::string& name)
	{
		if (!this) return &Actor(entt::null, *new entt::registry());

		if (!_entityRegistry) {
			CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?");
			return &Actor(entt::null, *new entt::registry());
		}
		auto actor = _entityRegistry->CreateActor(name);
		_Actors.push_back(actor);
		return actor;
	}

	void World::DestroyActor(const Actor& actor)
	{
		if (!this) return;

		if (!_entityRegistry) {
			CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?");
		}
		_entityRegistry->RemoveActor(actor);
	}

	std::vector<Actor*> World::GetAllActors()
	{
		return _Actors;
	}

}