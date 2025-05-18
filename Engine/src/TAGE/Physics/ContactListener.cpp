#include "tagepch.h"
#include "ContactListener.h"
#include "TAGE/ECS/Components/PhysicsComponents.h"

namespace TAGE::PHYSICS {
    void PhysicsContactListener::CheckContacts(btDynamicsWorld* world, entt::registry& registry)
    {
        int numManifolds = world->getDispatcher()->getNumManifolds();
        for (int i = 0; i < numManifolds; i++) {
            btPersistentManifold* contactManifold = world->getDispatcher()->getManifoldByIndexInternal(i);
            const btCollisionObject* obA = contactManifold->getBody0();
            const btCollisionObject* obB = contactManifold->getBody1();

            if (contactManifold->getNumContacts() > 0) {
                auto* compA = static_cast<ECS::RigidBodyComponent*>(obA->getUserPointer());
                auto* compB = static_cast<ECS::RigidBodyComponent*>(obB->getUserPointer());

                if (compA && compB) {
                    auto& colA = registry.get<ECS::ColliderComponent>(compA->Owner->GetHandle());
                    auto& colB = registry.get<ECS::ColliderComponent>(compB->Owner->GetHandle());

                    bool isTrigger = colA.IsTrigger() || colB.IsTrigger();

                    Contacts.push_back({
                        compA->Owner,
                        compB->Owner,
                        isTrigger ? ContactType::Trigger : ContactType::Collision
                        });
                }
            }
        }
    }
}