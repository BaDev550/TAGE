#include "tagepch.h"
#include "Actor.h"
#include "TAGE/ECS/ECS/Components/BaseComponents.h"

namespace TAGE::ECS {
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
            return;
        }

        if (!HasComponent<RelationshipComponent>())
            AddComponent<RelationshipComponent>();

        auto& rel = GetComponent<RelationshipComponent>();
        std::cout << "Setting parent for Actor " << GetComponent<TagComponent>().name << " to " << parent->GetComponent<TagComponent>().name << std::endl;

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