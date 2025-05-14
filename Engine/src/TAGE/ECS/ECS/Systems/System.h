#pragma once

#include "TAGE/ECS/ECS/Components/BaseComponents.h"
#include "TAGE/ECS/ECS/Components/PhysicsComponents.h"
#include "TAGE/ECS/ECS/Components/RenderingComponents.h"

#include "TAGE/ECS/Objects/Object.h"
#include "TAGE/ECS/Objects/Actor.h"
#include "TAGE/Renderer/Renderer.h"
#include "TAGE/Physics/PhysicsWorld.h"
#include "TAGE/Physics/ContactListener.h"
#include "TAGE/Physics/PhysicsDebugRenderer.h"
#include "TAGE/Physics/Raycaster.h"

#include "TAGE/Core/Profiler/Profiler.h"

#define PHYSICS_DEBUGER_INCLUDED
namespace TAGE::ECS {
    using namespace RENDERER;
    enum class SystemUpdateMode { EDITOR, GAME };

    class System : public Object {
    public:
        virtual ~System() = default;
        virtual void Update(entt::registry& registry, float dt, SystemUpdateMode mode) = 0;
    };

    class TransformSystem : public System {
    public:
        void Update(entt::registry& registry, float dt, SystemUpdateMode mode) override;
    private:
        void UpdateTransformRecursive(entt::registry& registry, entt::entity entity, const glm::mat4& parentTransform);
    };

    class RenderSystem : public System {
    public:
        RenderSystem(Renderer* renderer);

        void Update(entt::registry& registry, float dt, SystemUpdateMode mode) override;

        void RenderScene(entt::registry& registry, float dt, const MEM::Ref<Shader>& shader);
        void PassLightsToShader(entt::registry& registry);
    private:
        MEM::Ref<Shader> _Shader;
        MEM::Ref<Shader> _ShadowShader;

        Renderer* _Renderer;
    };

    class PhysicsSystem : public System {
    public:
        PhysicsSystem(PHYSICS::PhysicsWorld& world);
        
        void Update(entt::registry& registry, float dt, SystemUpdateMode mode) override;
        btRigidBody* CreateRigidBody(Actor* actor, float mass);
    private:
        PHYSICS::PhysicsWorld& _PhysicsWorld;
        PHYSICS::PhysicsContactListener _ContactListener;
    };
}