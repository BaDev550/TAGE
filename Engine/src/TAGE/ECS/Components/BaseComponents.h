#pragma once

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "TAGE/ECS/Objects/Entity.h"
#include <vector>

namespace TAGE::ECS {
	struct TagComponent
	{
	public:
		std::string name;
		std::vector<std::string> tags;
		TagComponent(const std::string _name) : name(_name) {}
		TagComponent() {}
	};

	struct TransformComponent {
	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::vec3 LocalPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 LocalRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 LocalScale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 WorldMatrix = glm::mat4(1.0f);

		glm::mat4 GetMatrix() const {
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), (LocalPosition + Position));
			glm::mat4 rotation = glm::eulerAngleYXZ(
				glm::radians((LocalRotation.x + Rotation.y)),
				glm::radians((LocalRotation.y + Rotation.x)),
				glm::radians((LocalRotation.z + Rotation.z))
			);
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), (LocalScale * Scale));
			return translation * rotation * scale;
		}

		void UpdateWorldMatrix(const glm::mat4& parentWorldMatrix) {
			WorldMatrix = parentWorldMatrix * GetMatrix();
		}
	};

	struct RelationshipComponent {
		entt::entity Parent = entt::null;
		std::vector<entt::entity> Children;
	};
}