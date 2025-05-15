#pragma once

#include "Actor.h"
#include "GameObjectManager.h"
#include "TAGE/ECS/ECS/Components/BaseComponents.h"
#include "glm/glm.hpp"

namespace TAGE::ECS {
	enum class ObjectType {
		OBJECT_STATIC = 0,
		OBJECT_PAWN,
		OBJECT_WEAPON,
		OBJECT_CAMERA
	};

	class GameObject : public Actor {
	public:
		GameObject(const std::string& name, ObjectType type, GameObjectManager* manager = nullptr);
		virtual ~GameObject();

		virtual void Tick(float deltaTime) {}

		inline ObjectType GetType() const { return _Type; }

		glm::vec3& GetForwardVector();
		inline glm::vec3& GetWorldLocation() { return GetComponent<TransformComponent>().Position; }
		inline glm::vec3& GetWorldRotation() { return GetComponent<TransformComponent>().Rotation; }
		inline glm::vec3& GetWorldScale() { return GetComponent<TransformComponent>().Scale; }

		inline glm::vec3& GetLocalLocation() { return GetComponent<TransformComponent>().LocalPosition; }
		inline glm::vec3& GetLocalRotation() { return GetComponent<TransformComponent>().LocalRotation; }
		inline glm::vec3& GetLocalScale() { return GetComponent<TransformComponent>().LocalScale; }

		void AddTag(const std::string& tag);
		bool HasTag(const std::string& tag);
		std::string GetClassName() const {
			std::string name = typeid(*this).name();
			const std::string prefix = "class ";
			if (name.find(prefix) == 0)
				name = name.substr(prefix.size());
			return name;
		}

		bool IsGameObject() const override { return true; }
	protected:
		ObjectType _Type;
		GameObjectManager* _manager;
		bool _isRegistered = false;
	};
}