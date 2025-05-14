#include "tagepch.h"
#include "GameObject.h"
#include "TAGE/Core/Application/Application.h"

namespace TAGE::ECS {
	GameObject::GameObject(const std::string& name, ObjectType type, GameObjectManager* manager) :
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
	GameObject::~GameObject() {
		if (_manager && _isRegistered) {
			_manager->RemoveGameObject(this);
			_isRegistered = false;
		}
	}

	glm::vec3& GameObject::GetForwardVector() {
		auto& rotation = GetComponent<TransformComponent>().Rotation;
		glm::vec3 rotationRadians = glm::radians(rotation);
		glm::quat q = glm::quat(rotation);
		return glm::normalize(q * glm::vec3(0, 0, -1));
	}

	void GameObject::AddTag(const std::string& tag) {
		auto& tc = GetComponent<TagComponent>();
		if (!tag.empty())
			tc.tags.push_back(tag);
	}

	bool GameObject::HasTag(const std::string& tag) {
		auto& tc = GetComponent<TagComponent>();
		for (auto& tc_tag : tc.tags)
			if (tc_tag == tag)
				return true;
		return false;
	}
}