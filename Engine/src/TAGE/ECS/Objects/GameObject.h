#pragma once
#include "Actor.h"
#include "TAGE/Core/Application/Application.h"
#include "GameObjectManager.h"

namespace TAGE::ECS {
	enum class ObjectType {
		OBJECT_DOOR,
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

	protected:
		ObjectType _Type;
		GameObjectManager* _manager;
		bool _isRegistered = false;
	};
}