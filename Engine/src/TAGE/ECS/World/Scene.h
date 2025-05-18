#pragma once

#include "TAGE/ECS/Objects/Entity.h"
#include "TAGE/Core/Utilities/Timeline.h"
#include "TAGE/ECS/Objects/EntityRegistry.h"
#include "TAGE/Physics/PhysicsWorld.h"
#include "entt/entt.hpp"
#include <vector>
#include <string>

namespace TAGE::ECS {

	class System;
	class PhysicsSystem;
	class RenderSystem;
	enum class SystemUpdateMode;
	class Scene : public std::enable_shared_from_this<Scene> {
	public:
		Scene() = default;
		virtual ~Scene() = default;
		Scene(const Scene&) = delete;
		Scene& operator=(const Scene&) = delete;

		virtual void Init();
		virtual void Tick(float dt) {}
		virtual void Shutdown() {}

		Entity* CreateEntity(const std::string& name = "Entity");
		bool DestroyEntity(const Entity& entity);
		Entity* FindEntityByName(const std::string& name);
		Entity* FindEntityByID(entt::entity id);
		std::vector<MEM::Ref<Entity>> GetEntities() const { return _entities; }
		std::vector<MEM::Ref<Entity>> GetEntitiesByTag(const std::string& tag);

		void AddSystem(const MEM::Ref<System>& system);
		void UpdateSystems(float dt, SystemUpdateMode mode);
		void FixedUpdateSystems(float dt, SystemUpdateMode mode);
		void Clear();

		ECS::PhysicsSystem& GetPhysicsSystem() const;
		ECS::RenderSystem& GetRenderSystem() const;
		EntityRegistry& GetRegistry() { return *_entityRegistry; }
		PHYSICS::PhysicsWorld& GetPhysicsWorld() { return _PWorld; }
		const MEM::Scope<EntityRegistry>& GetRegistryScope() const { return _entityRegistry; }

		template<typename T>
		MEM::Ref<T> GetSystem() const {
			for (auto& sys : _systems) {
				MEM::Ref<T> casted = std::dynamic_pointer_cast<T>(sys);
				if (casted)
					return casted;
			}
			return nullptr;
		}
	protected:
		MEM::Scope<EntityRegistry> _entityRegistry;
		std::vector<MEM::Ref<System>> _systems;
		std::vector<MEM::Ref<Entity>> _entities;
		PHYSICS::PhysicsWorld _PWorld;
	};

}
