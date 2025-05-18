#include "tagepch.h"
#include "System.h"

namespace TAGE::ECS {
#ifndef PHYSICS_DEBUGER_INCLUDED
    PHYSICS::DEBUG::PhysicsDebugRenderer _PhysicsDebugRenderer;
#else
    extern PHYSICS::DEBUG::PhysicsDebugRenderer _PhysicsDebugRenderer;
#endif

    PhysicsSystem::PhysicsSystem() : _PhysicsWorld(PHYSICS::PhysicsWorld()) {
    }

    void PhysicsSystem::Init()
    {
        if (auto scene = _Scene.lock()) {
            _PhysicsWorld = scene->GetPhysicsWorld();
            _PhysicsWorld.GetWorld()->setDebugDrawer(&_PhysicsDebugRenderer);
            PHYSICS::RAYCAST::Raycaster::Init(_PhysicsWorld, _PhysicsDebugRenderer);
        }
    }

    void PhysicsSystem::Update(float dt)
    {
    }

    btCollisionShape* CreateShapeFromCollider(const ColliderComponent& collider, const MeshComponent& mesh)
    {
        btBvhTriangleMeshShape* meshShape;
        btTriangleMesh* triangleMesh;

        switch (collider.Shape)
        {
        case ColliderShapeType::BOX:
            return new btBoxShape(btVector3(
                collider.Size.x * 0.5f,
                collider.Size.y * 0.5f,
                collider.Size.z * 0.5f
            ));

        case ColliderShapeType::SPHERE:
            return new btSphereShape(collider.Size.x * 0.5f);

        case ColliderShapeType::CAPSULE:
            return new btCapsuleShape(
                collider.Size.x * 0.5f,
                collider.Size.y
            );

        case ColliderShapeType::MESH:
            ENGINE_ASSERT(false, "Unknown collider shape!");
            return nullptr;
        default:
            ENGINE_ASSERT(false, "Unknown collider shape!");
            return nullptr;
        }
    }

    void PhysicsSystem::FixedUpdate(float dt)
    {
        if (auto scene = _Scene.lock()) {
            const auto registry = scene->GetRegistry().GetRegistry();
            TE_PROFILE_SCOPE("Physics System");
            //_PhysicsWorld.GetWorld()->debugDrawWorld();

            //auto rigidView = registry.view<TransformComponent, RigidBodyComponent>();
            //for (auto entity : rigidView) {
            //    auto& tc = rigidView.get<TransformComponent>(entity);
            //    auto& rb = rigidView.get<RigidBodyComponent>(entity);

            //    glm::vec3 position = tc.Position;
            //    glm::vec3 rotation = tc.Rotation;

            //    btTransform transform;
            //    transform.setOrigin(btVector3(position.x, position.y, position.z));

            //    glm::quat q = glm::quat(glm::radians(rotation));
            //    transform.setRotation(btQuaternion(q.x, q.y, q.z, q.w));

            //    rb.Body->getMotionState()->setWorldTransform(transform);
            //    rb.Body->setWorldTransform(transform);
            //}
            //RefreshDirtyBodies(registry);
            //return;

            RefreshDirtyBodies(*registry);
            scene->GetPhysicsWorld().StepSimulation(dt);

            auto rigidView = registry->view<TransformComponent, RigidBodyComponent>();

            for (auto entity : rigidView) {
                auto& transform = rigidView.get<TransformComponent>(entity);
                auto& rb = rigidView.get<RigidBodyComponent>(entity);

                if (rb.Body->isKinematicObject() || rb.Body->isStaticObject()) {
                    glm::vec3 position = transform.Position;
                    glm::vec3 rotation = transform.Rotation;

                    btTransform transform;
                    transform.setOrigin(btVector3(position.x, position.y, position.z));

                    glm::quat q = glm::quat(glm::radians(rotation));
                    transform.setRotation(btQuaternion(q.x, q.y, q.z, q.w));

                    rb.Body->getMotionState()->setWorldTransform(transform);
                    rb.Body->setWorldTransform(transform);
                }

                btTransform bulletTransform;
                rb.Body->getMotionState()->getWorldTransform(bulletTransform);
                btVector3 pos = bulletTransform.getOrigin();
                btQuaternion rot = bulletTransform.getRotation();

                glm::quat q(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
                transform.Position = glm::vec3(pos.getX(), pos.getY(), pos.getZ());
                transform.Rotation = glm::eulerAngles(q) * (180.0f / glm::pi<float>());
            }
        }
    }

    void PhysicsSystem::UpdateRigidBodyShape(entt::registry& registry, entt::entity entity)
    {
        auto& rb = registry.get<RigidBodyComponent>(entity);
        auto& collider = registry.get<ColliderComponent>(entity);

        btCollisionShape* newShape;
        if (registry.any_of<StaticMeshComponent>(entity))
            newShape = CreateShapeFromCollider(collider, registry.get<StaticMeshComponent>(entity));
        else
            newShape = CreateShapeFromCollider(collider, registry.get<SkeletalMeshComponent>(entity));

        delete rb.CollisionShape;

        rb.CollisionShape = newShape;
        int flags = rb.Body->getCollisionFlags();
        switch (collider.ResponseType)
        {
        case CollisionResponseType::NONE:
            flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
            break;
        case CollisionResponseType::OVERLAP:
            flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
            break;
        case CollisionResponseType::BLOCK:
            break;
        }
        rb.Body->setCollisionFlags(flags);

        rb.Body->setCollisionShape(newShape);

        btVector3 localInertia(0, 0, 0);
        float mass = rb.BodyType == PhysicsBodyType::DYNAMIC ? 1.0f : 0.0f;

        if (mass > 0.0f)
            newShape->calculateLocalInertia(mass, localInertia);

        rb.Body->setMassProps(mass, localInertia);
        rb.Body->updateInertiaTensor();
    }

    void PhysicsSystem::RefreshDirtyBodies(entt::registry& registry)
    {
        auto view = registry.view<ColliderComponent, RigidBodyComponent>();
        for (auto entity : view) {
            auto& collider = view.get<ColliderComponent>(entity);

            if (!collider.Dirty)
                continue;

            UpdateRigidBodyShape(registry, entity);
            collider.Dirty = false;
        }
    }

    btRigidBody* PhysicsSystem::CreateRigidBody(Entity* actor, float mass)
    {
        auto& transform = actor->GetComponent<TransformComponent>();
        auto& collider = actor->GetComponent<ColliderComponent>();

        btCollisionShape* shape;
        if (actor->HasComponent<StaticMeshComponent>())
            shape = CreateShapeFromCollider(collider, actor->GetComponent<StaticMeshComponent>());
        else
            shape = CreateShapeFromCollider(collider, actor->GetComponent<SkeletalMeshComponent>());

        btTransform startTransform;
        startTransform.setIdentity();
        startTransform.setOrigin(btVector3(transform.Position.x, transform.Position.y, transform.Position.z));
        btQuaternion rotation = btQuaternion::getIdentity();

        btDefaultMotionState* motion = new btDefaultMotionState(startTransform);

        btVector3 inertia(0, 0, 0);
        if (mass > 0.0f)
            shape->calculateLocalInertia(mass, inertia);

        btRigidBody::btRigidBodyConstructionInfo info(mass, motion, shape, inertia);
        btRigidBody* body = new btRigidBody(info);
        body->setAngularFactor(btVector3(0, 0, 0));

        int flags = body->getCollisionFlags();
        switch (collider.ResponseType)
        {
        case CollisionResponseType::NONE:
            flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
            break;
        case CollisionResponseType::OVERLAP:
            flags |= btCollisionObject::CF_NO_CONTACT_RESPONSE;
            break;
        case CollisionResponseType::BLOCK:
            break;
        }
        body->setCollisionFlags(flags);

        _PhysicsWorld.GetWorld()->addRigidBody(body);
        actor->AddComponent<RigidBodyComponent>(body, motion, shape, actor);
        return body;
    }
}