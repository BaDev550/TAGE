#pragma once

#include "TAGE/TAGE.h"
#include "TAGE/TObjects.h"
#include "TAGE/TComponents.h"

class Wall : public TGameObject {
public:
    Wall() : TGameObject("Wall", TObjectType::OBJECT_STATIC) {}

    Wall(const std::string& meshPath, const glm::vec3& position, const glm::vec3& scale) : TGameObject("Wall", TObjectType::OBJECT_STATIC) {
        auto& mesh = AddComponent<TStaticMeshComponent>(meshPath);
        auto& transform = GetComponent<TTransformComponent>();
        auto& collider = AddComponent<TColliderComponent>();
        collider.Size = scale;
        transform.Position = position;
        transform.Scale = scale / 2.0f;
        GetWorld().GetPhysicsSystem().CreateRigidBody(this, 0.0f);
    }
};

REGISTER_GAMEOBJECT_CLASS(Wall);