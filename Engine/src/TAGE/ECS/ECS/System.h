#pragma once

#include "Components.h"
#include "entt/entt.hpp"
#include "TAGE/ECS/Objects/Object.h"
#include "TAGE/ECS/Objects/Actor.h"
#include "TAGE/Renderer/Renderer.h"
#include "TAGE/Physics/PhysicsWorld.h"
#include "TAGE/Physics/ContactListener.h"
#include "TAGE/Physics/PhysicsDebugRenderer.h"
#include "TAGE/Physics/Raycaster.h"

namespace TAGE::ECS {
    enum class SystemUpdateMode {
        Editor,
        Game
    };

    class System : public Object {
    public:
        virtual ~System() = default;
        virtual void Update(entt::registry& registry, float dt, SystemUpdateMode mode) = 0;
    };

    class TransformSystem : public System {
    public:
        void Update(entt::registry& registry, float dt, SystemUpdateMode mode) override {
            auto view = registry.view<TransformComponent, RelationshipComponent>();

            for (auto entity : view) {
                auto& rel = view.get<RelationshipComponent>(entity);

                if (rel.Parent == entt::null) {
                    UpdateTransformRecursive(registry, entity, glm::mat4(1.0f)); 
                }
            }
        }

    private:
        void UpdateTransformRecursive(entt::registry& registry, entt::entity entity, const glm::mat4& parentTransform) {
            auto& transform = registry.get<TransformComponent>(entity);

            transform.UpdateWorldMatrix(parentTransform);

            if (registry.any_of<RelationshipComponent>(entity)) {
                auto& rel = registry.get<RelationshipComponent>(entity);

                for (auto child : rel.Children) {
                    if (registry.valid(child)) {
                        UpdateTransformRecursive(registry, child, transform.WorldMatrix); 
                    }
                }
            }
        }
    };

    class RenderSystem : public System {
    public:
        RenderSystem(RENDERER::Renderer* renderer);

        void Update(entt::registry& registry, float dt, SystemUpdateMode mode) override;

        void RenderScene(entt::registry& registry, float dt, const MEM::Ref<RENDERER::Shader>& shader);
        void PassLightsToShader(entt::registry& registry);
        void UpdateCamera(entt::registry& registry);
    private:
        MEM::Ref<RENDERER::Shader> _Shader;
        MEM::Ref<RENDERER::Shader> _ShadowShader;

        RENDERER::Renderer* _Renderer;
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