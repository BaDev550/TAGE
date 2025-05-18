#pragma once

#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Core/Utilities/Timeline.h"

#include "TAGE/Renderer/Camera.h"
#include "TAGE/ECS/Systems/System.h"
#include "TAGE/Physics/PhysicsWorld.h"
#include "TAGE/ECS/Objects/EntityRegistry.h"
#include "TAGE/ECS/Objects/Entity.h"
#include "TAGE/ECS/World/Scene.h"

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

		void SetScene(MEM::Ref<ECS::Scene>& scene) { _CurrentScene = scene; }
		MEM::Ref<ECS::Scene> GetScene() const { return _CurrentScene; }
	private:
		MEM::Ref<ECS::Scene> _CurrentScene = nullptr;
	};
}