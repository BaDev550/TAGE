#pragma once

#include "TAGE/ECS/Objects/Actor.h"
#include <btBulletDynamicsCommon.h>

namespace TAGE::PHYSICS {
	class PhysicsContactListener
	{
    public:
        enum class ContactType {
            Collision,
            Trigger
        };

        struct Contact {
            ECS::Actor* EntityA;
            ECS::Actor* EntityB;
            ContactType Type;
        };

        std::vector<Contact> Contacts;

        void Clear() { Contacts.clear(); }

        void CheckContacts(btDynamicsWorld* world, entt::registry& registry);
	};
}