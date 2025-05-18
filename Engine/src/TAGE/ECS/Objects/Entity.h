#pragma once

#include "entt/entt.hpp"
#include <string>
#include <type_traits>

namespace TAGE {
	class Entity {
	public:
		Entity() = default;
		Entity(entt::entity handle, entt::registry* registry)
			: _handle(handle), _registry(registry) {
		}
		virtual ~Entity() = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) { return _registry->emplace<T>(_handle, std::forward<Args>(args)...); }

        template<typename T>
        bool HasComponent() const { return _registry->any_of<T>(_handle); }

        template<typename T>
        T& GetComponent() { return _registry->get<T>(_handle); }

        template<typename T>
        void RemoveComponent() { _registry->remove<T>(_handle); }


        bool IsValid() const { return _registry && _registry->valid(_handle); }
        entt::entity GetHandle() const { return _handle; }

        operator bool() const { return IsValid(); }
        bool operator==(const Entity& other) const { return _handle == other._handle && _registry == other._registry; }
        bool operator!=(const Entity& other) const { return !(*this == other); }
    private:
        entt::entity _handle{ entt::null };
        entt::registry* _registry = nullptr;
    };

}