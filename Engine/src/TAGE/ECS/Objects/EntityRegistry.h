#pragma once

#include "TAGE/ECS/Objects/Entity.h"
#include "TAGE/ECS/Components/BaseComponents.h"

namespace TAGE {
    using namespace ECS;
    class EntityRegistry {
    public:
        EntityRegistry() {
            _registry = MEM::CreateScope<entt::registry>();
        }

        ~EntityRegistry() {
        }

        MEM::Ref<Entity> CreateActor(const std::string& name = "Actor") {
            entt::entity e = _registry->create();
            auto actor = MEM::CreateRef<Entity>(e, _registry.get());
            actor->AddComponent<TagComponent>(name);
            actor->AddComponent<TransformComponent>();
            return actor;
        }
		MEM::Ref<Entity> FindActorByName(const std::string& name) {
			auto view = _registry->view<TagComponent>();
			for (auto entity : view) {
				if (_registry->get<TagComponent>(entity).name == name) {
					return MEM::CreateRef<Entity>(entity, _registry.get());
				}
			}
			return nullptr;
		}
		MEM::Ref<Entity> FindActorByID(entt::entity id) {
			auto view = _registry->view<TagComponent>();
			for (auto entity : view) {
				if (entity == id) {
					return MEM::CreateRef<Entity>(entity, _registry.get());
				}
			}
			return nullptr;
		}
		std::vector<MEM::Ref<Entity>> FindActorsByTag(const std::string& tag) {
			std::vector<MEM::Ref<Entity>> actors;
			auto view = _registry->view<TagComponent>();
			for (auto entity : view) {
                for (auto& t : _registry->get<TagComponent>(entity).tags) {
                    if (t == tag) {
                        actors.push_back(MEM::CreateRef<Entity>(entity, _registry.get()));
                    }
                }
			}
			return actors;
		}
        void RemoveActor(const Entity& actor) { _registry->destroy(actor.GetHandle()); }

        entt::registry* GetRegistry() { return _registry.get(); }
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
        MEM::Scope<entt::registry> _registry;
    };
}