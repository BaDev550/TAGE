#pragma once

#include "Object.h"
#include "entt/entt.hpp"

namespace TAGE::ECS {
    class Actor : public Object {
    public:
        Actor(Actor* Actor)
            : _entity(Actor->GetEntity()), _registry(Actor->_registry) {
        }
        Actor(entt::entity handle, entt::registry& registry)
            : _entity(handle), _registry(registry) {
        }
        virtual ~Actor() = default;

        template<typename T, typename... Args>
        T& AddComponent(Args&&... args) {
            return _registry.emplace<T>(_entity, std::forward<Args>(args)...);
        }

        template<typename T>
        bool HasComponent() const {
            return _registry.any_of<T>(_entity);
        }

        template<typename T>
        T& GetComponent() {
            if(HasComponent<T>())
                return _registry.get<T>(_entity);
        }

        bool IsValid() {
            return _registry.valid(_entity);
        }

        entt::entity GetEntity() const { return _entity; }

        operator bool() const {
            return _registry.valid(_entity);
        }

        inline bool operator==(Actor other) {
            return other.GetEntity() == this->GetEntity();
        }

        inline void operator=(Actor other) {
            this->_entity = other.GetEntity();
        }

        virtual void Tick(float deltaTime) override {};

        void AddChild(Actor* child);
        void SetParent(Actor* parent);
        Actor* GetParent() { 
            if (!_Parent) return nullptr;
            return _Parent;
        }

    private:
        entt::entity _entity;
        entt::registry& _registry;
        Actor* _Parent = nullptr;
    };
}