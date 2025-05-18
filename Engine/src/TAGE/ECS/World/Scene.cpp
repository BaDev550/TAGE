#include "tagepch.h"
#include "Scene.h"
#include "TAGE/ECS/Systems/System.h"

namespace TAGE::ECS {
	void Scene::Init()
	{
		_entityRegistry = MEM::CreateScope<EntityRegistry>();
	}

	Entity* Scene::CreateEntity(const std::string& name)
	{
		if (!this) return &Entity(entt::null, new entt::registry());
		if (!_entityRegistry) { CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?"); return &Entity(entt::null, new entt::registry()); }

		auto actor = _entityRegistry->CreateActor(name);
		Entity* actorPtr = actor.get();
		if (actor) {
			_entities.push_back(std::move(actor));
			CORE_LOG_INFO("Creating actor {}", name);
		}
		else {
			CORE_LOG_ERROR("Actor with name {} already exists.", name);
		}

		return actorPtr;
	}

	bool Scene::DestroyEntity(const Entity& entity)
	{
		if (!this) return false;
		if (!_entityRegistry) { CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?"); return false; }

		_entityRegistry->RemoveActor(entity);

		auto it = std::find_if(_entities.begin(), _entities.end(), [&](const MEM::Ref<Entity>& e) {
			return e->GetHandle() == entity.GetHandle();
			});
		if (it != _entities.end())
		{
			_entities.erase(it);
		}
		else
		{
			CORE_LOG_WARN("Entity to destroy not found in _entities");
		}
		return true;
	}

	Entity* Scene::FindEntityByName(const std::string& name)
	{
		if (!this) return nullptr;
		if (!_entityRegistry) { CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?"); return nullptr; }
		auto entity = _entityRegistry->FindActorByName(name);
		if (entity) {
			return entity.get();
		}
		else {
			CORE_LOG_ERROR("Entity with name {} not found.", name);
			return nullptr;
		}
	}

	Entity* Scene::FindEntityByID(entt::entity id)
	{
		if (!this) return nullptr;
		if (!_entityRegistry) { CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?"); return nullptr; }
		auto entity = _entityRegistry->FindActorByID(id);
		if (entity) {
			return entity.get();
		}
		else {
			CORE_LOG_ERROR("Entity with id {} not found.", static_cast<int>(id));
			return nullptr;
		}
	}

	std::vector<MEM::Ref<Entity>> Scene::GetEntitiesByTag(const std::string& tag)
	{
		if (!this) return {};
		if (!_entityRegistry) { CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?"); return {}; }
		auto entitys = _entityRegistry->FindActorsByTag(tag);
		if (!entitys.empty()) {
			return entitys;
		}
		else {
			CORE_LOG_ERROR("No Entity with tag {}", tag);
			return {};
		}
	}

	void Scene::Clear()
	{
		if (!this) return;
		if (!_entityRegistry) { CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?"); return; }
		_entityRegistry->ClearEntitys();
	}

	ECS::PhysicsSystem& Scene::GetPhysicsSystem() const { return *GetSystem<ECS::PhysicsSystem>(); }
	ECS::RenderSystem& Scene::GetRenderSystem() const { return *GetSystem<ECS::RenderSystem>(); }

	void Scene::AddSystem(const MEM::Ref<System>& system)
	{
		if (!this) return;
		if (!_entityRegistry) { CORE_LOG_ERROR("EntityRegistry is null! Did you forget to call World::Init()?"); return; }
		if (system) {
			_systems.push_back(system);
			system->SetScene(shared_from_this());
			system->Init();
		}
		else {
			CORE_LOG_ERROR("System is null!");
		}
	}

	void Scene::UpdateSystems(float dt, SystemUpdateMode mode)
	{
		for (auto& system : _systems) {
			if (system->ShoudUpdate(mode))
				system->Update(dt);
		}
		Timeline::Update(dt);
	}

	void Scene::FixedUpdateSystems(float dt, SystemUpdateMode mode)
	{
		for (auto& system : _systems) {
			if (system->ShoudUpdate(mode))
				system->FixedUpdate(dt);
		}
	}
}