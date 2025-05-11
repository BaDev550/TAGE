#pragma once

#include <btBulletDynamicsCommon.h>

namespace TAGE::PHYSICS {
	class PhysicsWorld
	{
	public:
        PhysicsWorld();
        ~PhysicsWorld();

        void StepSimulation(float deltaTime);

        btDiscreteDynamicsWorld* GetWorld() const { return m_DynamicsWorld; }

    private:
        btBroadphaseInterface* m_Broadphase;
        btDefaultCollisionConfiguration* m_CollisionConfig;
        btCollisionDispatcher* m_Dispatcher;
        btSequentialImpulseConstraintSolver* m_Solver;
        btDiscreteDynamicsWorld* m_DynamicsWorld;
	};
}