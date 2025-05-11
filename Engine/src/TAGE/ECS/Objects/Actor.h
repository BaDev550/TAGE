#pragma once

#include "Object.h"
#include "entt/entt.hpp"

namespace TAGE::ECS {
    class Actor : public Object {
    public:
        Actor(Actor* actor)
            : _entity(actor->GetEntity()), _registry(actor->_registry) {
            OnCreate();
        }
        Actor(entt::entity handle, entt::registry& registry)
            : _entity(handle), _registry(registry) {
            OnCreate();
        }

        virtual ~Actor() {
            _registry.destroy(_entity);
        }

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

        virtual void Tick(float deltaTime) {};
        virtual void OnCreate() {};
        virtual void OnDestroy() {}

        virtual std::string GetName() const override { return "Actor"; }
    private:
        entt::entity _entity;
        entt::registry& _registry;
    };
}