#include "tagepch.h"
#include "PhysicsWorld.h"

namespace TAGE::PHYSICS {

    PhysicsWorld::PhysicsWorld() {
        m_Broadphase = new btDbvtBroadphase();
        m_CollisionConfig = new btDefaultCollisionConfiguration();
        m_Dispatcher = new btCollisionDispatcher(m_CollisionConfig);
        m_Solver = new btSequentialImpulseConstraintSolver();
        m_DynamicsWorld = new btDiscreteDynamicsWorld(m_Dispatcher, m_Broadphase, m_Solver, m_CollisionConfig);

        m_DynamicsWorld->setGravity(btVector3(0, -9.81f, 0));
    }

    PhysicsWorld::~PhysicsWorld() {
        delete m_DynamicsWorld;
        delete m_Solver;
        delete m_Dispatcher;
        delete m_CollisionConfig;
        delete m_Broadphase;
    }

    void PhysicsWorld::StepSimulation(float deltaTime) {
        m_DynamicsWorld->stepSimulation(deltaTime);
    }

}