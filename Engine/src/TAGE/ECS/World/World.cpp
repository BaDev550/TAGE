#include "tagepch.h"
#include "World.h"

namespace TAGE {
	World::~World()
	{
	}

	void World::Init()
	{
		_entityRegistry = MEM::CreateScope<EntityRegistry>();
	}

	void World::Update(float dt, SystemUpdateMode mode)
	{
		if (mode != SystemUpdateMode::LOADING) {
			for (auto& system : _systems)
				system->Update(*_entityRegistry->GetRegistry(), dt, mode);
		}
	}

	void World::FixedUpdate(float dt, SystemUpdateMode mode)
	{
		if (mode != SystemUpdateMode::LOADING) {
			Timeline::Update(dt);
		}
	}

	Entity* World::SpawnActor(const std::string& name)
	{
		if (!this) return &Entity(entt::null, new entt::registry());
		if (!_entityRegistry) { CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?"); return &Entity(entt::null, new entt::registry()); }

		auto actor = _entityRegistry->CreateActor(name);
		if (actor) {
			CORE_LOG_INFO("Creating actor {}", name);
		}
		else {
			CORE_LOG_ERROR("Actor with name {} already exists.", name);
		}

		_Entitys.push_back(actor);
		return actor.get();
	}

	Entity* World::FindEntityByName(const std::string& name)
	{
		return nullptr;
	}

	void World::DestroyActor(const Entity& actor)
	{
		if (!this) return;
		if (!_entityRegistry) { CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?"); }

		_entityRegistry->RemoveActor(actor);
	}
}