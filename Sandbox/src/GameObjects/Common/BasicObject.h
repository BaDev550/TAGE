#pragma once

#include "TAGE/TAGE.h"

class BasicObject : public TGameObject {
public:
    BasicObject() : TGameObject("BasicObject", TObjectType::OBJECT_STATIC) {
        auto& mesh = AddComponent<TStaticMeshComponent>("Assets/Models/Cube/Cube.obj");
        Transform = &GetComponent<TTransformComponent>();
        Transform->Scale = glm::vec3(0.5f);
    }
    ~BasicObject() {
        delete Transform;
    }

    TTransformComponent& GetTransform() const { return *Transform; }
private:
    TTransformComponent* Transform;
};
