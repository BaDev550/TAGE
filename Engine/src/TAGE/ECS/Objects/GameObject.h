#pragma once
#include "Actor.h"
#include "TAGE/Core/Application/Application.h"
#include "GameObjectManager.h"

namespace TAGE::ECS {
	enum class ObjectType {
		OBJECT_STATIC,
		OBJECT_CHARACTER,
		OBJECT_WEAPON,
		OBJECT_CAMERA
	};

	class GameObject : public Actor {
	public:
		GameObject(const std::string& name, ObjectType type, GameObjectManager* manager = nullptr) :
			Actor(Application::Get().GetScene().GetWorld().SpawnActor(name)),
			_manager(manager)
		{
			if (!_manager)
				_manager = &Application::Get().GetScene().GetWorld().GetGameObjectManager();

			if (_manager && !_isRegistered) {
				_manager->RegisterGameObject(this);
				_isRegistered = true;
			}
		}
		virtual ~GameObject() {
			if (_manager && _isRegistered) {
				_manager->RemoveGameObject(this);
				_isRegistered = false;
			}
		}

		inline ObjectType GetType() const { return _Type; }
		virtual void Tick(float deltaTime) {}

		glm::vec3& GetForwardVector() {
			auto& rotation = GetComponent<TransformComponent>().Rotation;
			glm::vec3 rotationRadians = glm::radians(rotation); 
			glm::quat q = glm::quat(rotation); 
			return glm::normalize(q * glm::vec3(0, 0, -1));
		}

		inline glm::vec3& GetWorldLocation() { return GetComponent<TransformComponent>().Position; }
	protected:
		ObjectType _Type;
		GameObjectManager* _manager;
		bool _isRegistered = false;
	};
}