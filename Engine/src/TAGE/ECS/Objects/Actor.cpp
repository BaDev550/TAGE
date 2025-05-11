#include "tagepch.h"
#include "Actor.h"
#include "TAGE/ECS/ECS/Components.h"

namespace TAGE::ECS {
    //Actor::~Actor()
    //{
    //    if (_registry.valid(_entity)) {
    //        if (_registry.any_of<RelationshipComponent>(_entity)) {
    //            auto& rel = _registry.get<RelationshipComponent>(_entity);
    //            for (auto child : rel.Children) {
    //                if (_registry.valid(child)) {
    //                    auto& childRel = _registry.get<RelationshipComponent>(child);
    //                    childRel.Parent = entt::null;
    //                }
    //            }
    //        }

    //        _registry.destroy(_entity);
    //    }
    //}

    void Actor::AddChild(Actor* child) {
        if (!HasComponent<RelationshipComponent>())
            AddComponent<RelationshipComponent>();

        auto& rel = GetComponent<RelationshipComponent>();
        if (std::find(rel.Children.begin(), rel.Children.end(), child->GetEntity()) != rel.Children.end()) {
            return;
        }

        rel.Children.push_back(child->GetEntity());

        if (!child->HasComponent<RelationshipComponent>())
            child->AddComponent<RelationshipComponent>();

        child->SetParent(this);
        child->GetComponent<RelationshipComponent>().Parent = GetEntity();
    }

    void Actor::SetParent(Actor* parent) {
        if (parent == this) {
            std::cout << "Parent is this" << std::endl;
            return;
        }

        if (!HasComponent<RelationshipComponent>())
            AddComponent<RelationshipComponent>();

        auto& rel = GetComponent<RelationshipComponent>();
        std::cout << "Setting parent for Actor " << GetComponent<TagComponent>().tag << " to " << parent->GetComponent<TagComponent>().tag << std::endl;

        if (rel.Parent != entt::null) {
            auto& currentParentRel = _registry.get<RelationshipComponent>(rel.Parent);
            auto it = std::find(currentParentRel.Children.begin(), currentParentRel.Children.end(), GetEntity());
            if (it != currentParentRel.Children.end()) {
                currentParentRel.Children.erase(it);
            }
        }

        rel.Parent = parent->GetEntity();

        if (!parent->HasComponent<RelationshipComponent>())
            parent->AddComponent<RelationshipComponent>();

        parent->GetComponent<RelationshipComponent>().Children.push_back(GetEntity());
        this->_Parent = parent;

        std::cout << "Parent set successfully" << std::endl;
    }
}