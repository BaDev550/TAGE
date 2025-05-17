#pragma once

#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Core/Utilities/Timeline.h"

#include "TAGE/Renderer/Camera.h"
#include "TAGE/ECS/Systems/System.h"
#include "TAGE/Physics/PhysicsWorld.h"
#include "TAGE/ECS/Objects/EntityRegistry.h"
#include "TAGE/ECS/Objects/Entity.h"

#include <vector>

#define SCENE_SAVE_FORMAT_JSON
namespace TAGE {
	using namespace ECS;
	class World
	{
	public:
		World() {}
		~World();

		void Init();
		void Update(float dt, SystemUpdateMode mode);
		void FixedUpdate(float dt, SystemUpdateMode mode);

		Entity* SpawnActor(const std::string& name = "Actor");
		Entity* FindEntityByName(const std::string& name);
		void DestroyActor(const Entity& actor);

		std::vector<MEM::Scope<Entity>> GetEntitys() const { return _Entitys; }
		void AddSystem(const MEM::Ref<System>& system) { _systems.push_back(system); }

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

		void Clear() { _entityRegistry->ClearEntitys(); }

		PHYSICS::PhysicsWorld& GetPhysicsWorld() { return _PWorld; }
		MEM::Scope<EntityRegistry>& GetRegistry() { return _entityRegistry; }
	private:
		MEM::Scope<EntityRegistry> _entityRegistry;
		std::vector<MEM::Ref<System>> _systems;
		std::vector<MEM::Scope<Entity>> _Entitys;
		PHYSICS::PhysicsWorld _PWorld;
	};
}