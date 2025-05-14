#pragma once

#include "TAGE/ECS/Objects/Actor.h"
#include "Components/BaseComponents.h"

namespace TAGE::ECS {
    class EntityRegistry {
    public:
        Actor* CreateActor(const std::string& name = "Actor") {
            entt::entity e = _registry.create();
            auto* actor = new Actor(e, _registry);
            actor->AddComponent<TagComponent>(name);
            actor->AddComponent<TransformComponent>();
            return actor;
        }

        void RemoveActor(const Actor& actor) {
            _registry.destroy(actor.GetEntity());
        }

        entt::registry& GetRegistry() { return _registry; }
        
        std::vector<entt::entity> GetAllEntities() {
            std::vector<entt::entity> entities;

            auto view = _registry.view<TagComponent, TransformComponent>();
                for (auto entity : view) {
                    entities.push_back(entity);
                }
                return entities;
        }

        void ClearEntitys() {
            auto view = _registry.view<TagComponent, TransformComponent>();
            for (auto entity : view) {
                _registry.destroy(entity);
            }
        }

    private:
        entt::registry _registry;
    };
}