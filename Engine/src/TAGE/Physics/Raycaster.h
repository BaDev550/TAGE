#pragma once

#include "PhysicsWorld.h"
#include "TAGE/ECS/Objects/Actor.h"
#include "glm/glm.hpp"

namespace TAGE::PHYSICS::DEBUG { class PhysicsDebugRenderer; }
namespace TAGE::PHYSICS::RAYCAST {
	enum class RayDrawType { FOR_FRAME = 0, FOR_DURATION };
	struct RaycastLine {
		glm::vec3 from;
		glm::vec3 to;
		glm::vec3 color;
		RayDrawType drawType;
		float time;
	};

	struct RaycastHit
	{
		ECS::Actor* actor;
		glm::vec3 point;
		glm::vec3 normal;
		float distance;
		bool hit;
	};

	class Raycaster
	{
	public:
		static void Init(PHYSICS::PhysicsWorld& world, PHYSICS::DEBUG::PhysicsDebugRenderer& _debugRenderer);
		static RaycastHit Raycast(const glm::vec3& from, const glm::vec3& to, RayDrawType draw = RayDrawType::FOR_FRAME, float draw_time = 0.0f);
	private:
		static PHYSICS::PhysicsWorld* _World;
		static PHYSICS::DEBUG::PhysicsDebugRenderer* _DebugRenderer;
	};
}