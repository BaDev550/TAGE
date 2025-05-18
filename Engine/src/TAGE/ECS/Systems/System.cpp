#include "tagepch.h"
#include "System.h"
#include "TAGE/Core/Application/Application.h"

namespace TAGE::ECS {
    PHYSICS::DEBUG::PhysicsDebugRenderer _PhysicsDebugRenderer;

    void TransformSystem::Update(float dt)
	{
		if (auto scene = _Scene.lock()) {
			TE_PROFILE_SCOPE("Transform System");
			const auto registry = scene->GetRegistry().GetRegistry();
			auto view = registry->view<TransformComponent, RelationshipComponent>();
			for (auto entity : view) {
				auto& rel = view.get<RelationshipComponent>(entity);
				if (rel.Parent == entt::null) {
					UpdateTransformRecursive(entity, glm::mat4(1.0f));
				}
			}
		}
    }

    void TransformSystem::UpdateTransformRecursive(entt::entity entity, const glm::mat4& parentTransform) {
		if (auto scene = _Scene.lock()) {
			const auto registry = scene->GetRegistry().GetRegistry();
			auto& transform = registry->get<TransformComponent>(entity);
			auto& rel = registry->get<RelationshipComponent>(entity);
			glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), transform.Position) *
				glm::toMat4(glm::quat(glm::radians(transform.Rotation))) *
				glm::scale(glm::mat4(1.0f), transform.Scale);
			glm::mat4 worldTransform = parentTransform * localTransform;
			transform.WorldMatrix = worldTransform;
			for (auto child : rel.Children) {
				UpdateTransformRecursive(child, worldTransform);
			}
		}
    }
}