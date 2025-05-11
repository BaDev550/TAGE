#include "tagepch.h"
#include "System.h"
#include "TAGE/Core/Application/Application.h"

namespace TAGE::ECS {
    PHYSICS::DEBUG::PhysicsDebugRenderer _PhysicsDebugRenderer;

    RenderSystem::RenderSystem(RENDERER::Renderer* renderer) {
        _Renderer = renderer;
        _Shader = ShaderLibrary::Get("core");
        _ShadowShader = ShaderLibrary::Get("shadow");
    }

    void RenderSystem::PassLightsToShader(entt::registry& registry) {

    }

    void RenderSystem::UpdateCamera(entt::registry& registry) {
        auto view = registry.view<TransformComponent, CameraComponent>();
        PassLightsToShader(registry);
        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            auto& camera = view.get<CameraComponent>(entity);
            transform.Position = camera.Camera->GetPosition();
            transform.Rotation = camera.Camera->GetRotation();
        }
    }

    void RenderSystem::RenderScene(entt::registry& registry, float dt, const MEM::Ref<RENDERER::Shader>& shader) {
        auto staticmeshview = registry.view<TransformComponent, StaticMeshComponent>();
        auto skeletalmeshview = registry.view<TransformComponent, SkeletalMeshComponent>();

        for (const auto& entity : staticmeshview) {
            auto& transform = registry.get<TransformComponent>(entity);
            auto& static_model = registry.get<StaticMeshComponent>(entity);
            static_model.Draw(shader, transform.GetMatrix());
        }

        for (const auto& entity : skeletalmeshview) {
            auto& transform = registry.get<TransformComponent>(entity);
            auto& skeletal_model = registry.get<SkeletalMeshComponent>(entity);

            if (registry.any_of<AnimatorComponent>(entity)) {
                auto& animator = registry.get<AnimatorComponent>(entity).AnimatorInstance;
                animator->UpdateAnimation(dt);

                auto transforms = animator->GetFinalBoneMatrices();
                for (int i = 0; i < transforms.size(); ++i)
                    _Shader->SetUniform("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
            }

            skeletal_model.Draw(shader, transform.GetMatrix());
        }
    }

    void RenderSystem::Update(entt::registry& registry, float dt, SystemUpdateMode mode)
    {
        PassLightsToShader(registry);
        auto cameraView = registry.view<CameraComponent>();

        if (!cameraView.empty()) {
            auto entity = *cameraView.begin();
            auto activeCam = registry.get<CameraComponent>(entity);

            activeCam.Camera->OnUpdate(dt);

            _Renderer->BeginShadowMap();
            RenderScene(registry, dt, _ShadowShader);
            _Renderer->EndShadowMap();

            _Renderer->BeginScene(activeCam.Camera);
            RenderScene(registry, dt, _Shader);
            _Renderer->EndScene();

            Application::Get().UpdateLayers(dt);
            _PhysicsDebugRenderer.Flush(dt);

            UpdateCamera(registry);
        }
    }

    PhysicsSystem::PhysicsSystem(PHYSICS::PhysicsWorld& world)
        : _PhysicsWorld(world) {
        _PhysicsWorld.GetWorld()->setDebugDrawer(&_PhysicsDebugRenderer);
        PHYSICS::RAYCAST::Raycaster::Init(_PhysicsWorld, _PhysicsDebugRenderer);
    }

    void PhysicsSystem::Update(entt::registry& registry, float dt, SystemUpdateMode mode)
    {
        _PhysicsWorld.StepSimulation(dt);
        _PhysicsWorld.GetWorld()->debugDrawWorld();

        auto rigidView = registry.view<TransformComponent, RigidBodyComponent>();
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

            transform.Position = glm::vec3(pos.getX(), pos.getY(), pos.getZ());
            glm::quat q(rot.getW(), rot.getX(), rot.getY(), rot.getZ());
            transform.Rotation = glm::eulerAngles(q) * (180.0f / glm::pi<float>());
        }
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

    btRigidBody* PhysicsSystem::CreateRigidBody(Actor* actor, float mass)
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