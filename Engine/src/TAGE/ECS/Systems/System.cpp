#include "tagepch.h"
#include "System.h"
#include "TAGE/Core/Application/Application.h"

namespace TAGE::ECS {
    PHYSICS::DEBUG::PhysicsDebugRenderer _PhysicsDebugRenderer;

    void TransformSystem::Update(entt::registry& registry, float dt, SystemUpdateMode mode) {
        auto view = registry.view<TransformComponent, RelationshipComponent>();

        for (auto entity : view) {
            auto& rel = view.get<RelationshipComponent>(entity);

            if (rel.Parent == entt::null) {
                UpdateTransformRecursive(registry, entity, glm::mat4(1.0f));
            }
        }
    }
    void TransformSystem::UpdateTransformRecursive(entt::registry& registry, entt::entity entity, const glm::mat4& parentTransform) {
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
}