#pragma once

#include "PhysicsWorld.h"
#include "TAGE/ECS/Objects/Actor.h"
#include "TAGE/ECS/Objects/GameObject.h"
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
		ECS::GameObject* actor;
		glm::vec3 point;
		glm::vec3 normal;
		float distance;
		bool hit;
	};

	class ClosestNotMeRayResultCallback : public btCollisionWorld::ClosestRayResultCallback {
	public:
		const btCollisionObject* ignoreObject;

		ClosestNotMeRayResultCallback(const btVector3& rayFromWorld, const btVector3& rayToWorld, const btCollisionObject* ignore)
			: btCollisionWorld::ClosestRayResultCallback(rayFromWorld, rayToWorld), ignoreObject(ignore) {}

		virtual bool needsCollision(btBroadphaseProxy* proxy0) const override {
			if (proxy0->m_clientObject == ignoreObject) return false;
			return btCollisionWorld::ClosestRayResultCallback::needsCollision(proxy0);
		}
	};

	class Raycaster
	{
	public:
		static void Init(PHYSICS::PhysicsWorld& world, PHYSICS::DEBUG::PhysicsDebugRenderer& _debugRenderer);
		static RaycastHit Raycast(const glm::vec3& from, const glm::vec3& to, bool ignoreSelf = true, RayDrawType draw = RayDrawType::FOR_FRAME, float draw_time = 0.0f);
		static thread_local ECS::Actor* CurrentCaller;
	private:
		static PHYSICS::PhysicsWorld* _World;
		static PHYSICS::DEBUG::PhysicsDebugRenderer* _DebugRenderer;
	};
}