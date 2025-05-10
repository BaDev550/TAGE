#pragma once

#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Renderer/Camera.h"
#include "TAGE/ECS/ECS/EntityRegistry.h"
#include "TAGE/ECS/ECS/System.h"
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
		std::vector<Actor*> GetAllActors();

		void AddSystem(MEM::Scope<System> system) {
			_systems.push_back(std::move(system));
		}

		void Clear() {
			_entityRegistry->ClearEntitys();
		}

		MEM::Scope<EntityRegistry>& GetRegistry() { return _entityRegistry; }
	private:
		MEM::Scope<EntityRegistry> _entityRegistry;
		std::vector<MEM::Scope<System>> _systems;
		std::vector<Actor*> _Actors;
	};
}