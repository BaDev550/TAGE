#pragma once

#include "TAGE/ECS/Objects/Entity.h"
#include "TAGE/ECS/Components/BaseComponents.h"

namespace TAGE {
    class EntityRegistry {
    public:
        EntityRegistry() {
            _registry = new entt::registry();
        }

        ~EntityRegistry() {
            delete _registry;
        }

        MEM::Scope<Entity> CreateActor(const std::string& name = "Actor") {
            entt::entity e = _registry->create();
            auto actor = MEM::CreateScope<Entity>(e, _registry);
            actor->AddComponent<TagComponent>(name);
            actor->AddComponent<TransformComponent>();
            return actor;
        }
        void RemoveActor(const Entity& actor) { _registry->destroy(actor.GetHandle()); }

        entt::registry* GetRegistry() { return _registry; }
        std::vector<entt::entity> GetAllEntities() {
            std::vector<entt::entity> entities;

            auto view = _registry->view<TagComponent, TransformComponent>();
                for (auto entity : view) {
                    entities.push_back(entity);
                }
                return entities;
        }

        void ClearEntitys() {
            auto view = _registry->view<TagComponent, TransformComponent>();
            for (auto entity : view) {
                _registry->destroy(entity);
            }
        }

    private:
        entt::registry* _registry;
    };
}