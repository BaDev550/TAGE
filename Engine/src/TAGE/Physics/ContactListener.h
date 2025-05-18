#pragma once

#include "TAGE/ECS/Objects/Entity.h"
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
            Entity* EntityA;
            Entity* EntityB;
            ContactType Type;
        };

        std::vector<Contact> Contacts;

        void Clear() { Contacts.clear(); }

        void CheckContacts(btDynamicsWorld* world, entt::registry& registry);
	};
}