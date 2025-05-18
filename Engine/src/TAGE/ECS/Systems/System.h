#pragma once

#include "TAGE/ECS/Components/BaseComponents.h"
#include "TAGE/ECS/Components/PhysicsComponents.h"
#include "TAGE/ECS/Components/RenderingComponents.h"

#include "TAGE/Renderer/Renderer.h"
#include "TAGE/Physics/PhysicsWorld.h"
#include "TAGE/Physics/ContactListener.h"
#include "TAGE/Physics/PhysicsDebugRenderer.h"
#include "TAGE/Physics/Raycaster.h"
#include "TAGE/ECS/World/Scene.h"

#include "TAGE/ECS/Objects/Entity.h"
#include "TAGE/Core/Profiler/Profiler.h"

#define PHYSICS_DEBUGER_INCLUDED
namespace TAGE::ECS {
    using namespace RENDERER;
    enum class SystemUpdateMode { EDITOR, GAME, LOADING };

    class System {
    public:
        virtual ~System() = default;
        virtual bool ShoudUpdate(SystemUpdateMode mode) = 0;
        virtual void Update(float dt) = 0;
		virtual void FixedUpdate(float dt) {}
        virtual void Init() {};
        void SetScene(const MEM::Ref<Scene>& scene) { _Scene = scene; }

    protected:
		MEM::Weak<Scene> _Scene;
    };

    class TransformSystem : public System {
    public:
        virtual bool ShoudUpdate(SystemUpdateMode mode) {
            switch (mode)
            {
            case TAGE::ECS::SystemUpdateMode::EDITOR: return true;
            case TAGE::ECS::SystemUpdateMode::GAME: return true;
            case TAGE::ECS::SystemUpdateMode::LOADING: return false;
            }
        };
        void Update(float dt) override;
    private:
        void UpdateTransformRecursive(entt::entity entity, const glm::mat4& parentTransform);
    };

    class RenderSystem : public System {
    public:
        RenderSystem();
        virtual void Init();

        virtual bool ShoudUpdate(SystemUpdateMode mode) {
            switch (mode)
            {
            case TAGE::ECS::SystemUpdateMode::EDITOR: return true;
            case TAGE::ECS::SystemUpdateMode::GAME: return true;
            case TAGE::ECS::SystemUpdateMode::LOADING: return false;
            }
        };
        void Update(float dt) override;

        void RenderScene(float dt, const MEM::Ref<Shader>& shader);
        void PassLightsToShader();
    private:
        MEM::Ref<Shader> _Shader;
        MEM::Ref<Shader> _ShadowShader;

        Renderer* _Renderer;
    };

    class PhysicsSystem : public System {
    public:
        PhysicsSystem();
        virtual void Init();

        virtual bool ShoudUpdate(SystemUpdateMode mode) {
            switch (mode)
            {
            case TAGE::ECS::SystemUpdateMode::EDITOR: return false;
            case TAGE::ECS::SystemUpdateMode::GAME: return true;
            case TAGE::ECS::SystemUpdateMode::LOADING: return false;
            }
        };
        void Update(float dt) override;
		void FixedUpdate(float dt) override;
        btRigidBody* CreateRigidBody(Entity* actor, float mass);
    private:
        PHYSICS::PhysicsWorld& _PhysicsWorld;
        PHYSICS::PhysicsContactListener _ContactListener;

        void UpdateRigidBodyShape(entt::registry& registry, entt::entity entity);
        void RefreshDirtyBodies(entt::registry& registry);
    };
}