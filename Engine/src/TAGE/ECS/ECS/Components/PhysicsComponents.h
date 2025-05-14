#pragma once

#include <btBulletDynamicsCommon.h>
#include "TAGE/ECS/Objects/Actor.h"
#include <glm/glm.hpp>

namespace TAGE::ECS {
	enum class ColliderShapeType { BOX, SPHERE, CAPSULE, MESH };
	enum class CollisionResponseType { NONE, OVERLAP, BLOCK };
	enum class PhysicsBodyType { STATIC = 0, DYNAMIC, KINEMATIC };

	struct ColliderComponent {
		ColliderShapeType Shape = ColliderShapeType::BOX;
		CollisionResponseType ResponseType = CollisionResponseType::BLOCK;
		glm::vec3 Size = glm::vec3(1.0f);
		bool Dirty = false;

		bool IsTrigger() const { return ResponseType == CollisionResponseType::OVERLAP; }
	};

	struct RigidBodyComponent {
		btRigidBody* Body = nullptr;
		btDefaultMotionState* MotionState = nullptr;
		btCollisionShape* CollisionShape = nullptr;
		PhysicsBodyType BodyType = PhysicsBodyType::DYNAMIC;
		Actor* Owner;

		RigidBodyComponent(btRigidBody* body, btDefaultMotionState* motion, btCollisionShape* shape, Actor* owner) : Body(body), MotionState(motion), CollisionShape(shape), Owner(owner) {
			body->setUserPointer(Owner);
		}

		void AddForce(const glm::vec3& force) {
			Body->activate();
			Body->applyCentralForce(btVector3(force.x, force.y, force.z));
		}

		void SetVelocity(const glm::vec3& vel) {
			Body->activate();
			Body->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
		}

		glm::vec3& GetVelocity() const {
			return glm::vec3(Body->getLinearVelocity().getX(), Body->getLinearVelocity().getY(), Body->getLinearVelocity().getZ());
		}
	};
}