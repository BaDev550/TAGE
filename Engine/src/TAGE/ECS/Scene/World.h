#pragma once

#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Renderer/Camera.h"
#include "TAGE/ECS/ECS/EntityRegistry.h"
#include "TAGE/ECS/ECS/System.h"
#include "TAGE/Physics/PhysicsWorld.h"
#include "TAGE/ECS/Objects/GameObjectManager.h"

#include <vector>

namespace TAGE::ECS {
	class World
	{
	public:
		World() {}
		~World();

		void Init();
		void Update(float dt, SystemUpdateMode mode);
		
		Actor* SpawnActor(const std::string& name = "Actor");
		void DestroyActor(const Actor& actor);
		void RegisterActor(Actor* actor) {}
		std::vector<Actor*> GetAllActors();

		void AddSystem(const MEM::Ref<System>& system) {
			_systems.push_back(system);
		}

		template<typename T>
		MEM::Ref<T> GetSystem() const {
			for (auto& sys : _systems) {
				MEM::Ref<T> casted = std::dynamic_pointer_cast<T>(sys);
				if (casted)
					return casted;
			}
			return nullptr;
		}

		ECS::PhysicsSystem& GetPhysicsSystem() const { return *GetSystem<ECS::PhysicsSystem>(); }
		ECS::RenderSystem& GetRenderSystem() const { return *GetSystem<ECS::RenderSystem>(); }
		GameObjectManager& GetGameObjectManager() const { return *_GameObjectManager; }

		void Clear() {
			_entityRegistry->ClearEntitys();
		}

		PHYSICS::PhysicsWorld& GetPhysicsWorld() { return _PWorld; }
		MEM::Scope<EntityRegistry>& GetRegistry() { return _entityRegistry; }
	private:
		MEM::Scope<EntityRegistry> _entityRegistry;
		MEM::Scope<GameObjectManager> _GameObjectManager;
		std::vector<MEM::Ref<System>> _systems;
		std::vector<Actor*> _Actors;
		PHYSICS::PhysicsWorld _PWorld;
	};
}