#pragma once

#include "TAGE/TAGE.h"
#include "TAGE/TObjects.h"
#include "TAGE/TComponents.h"
#include "../../Interfaces/Interaction.h"

enum class EDoorState {
    OPENED,
    CLOSED
};

class Door : public TGameObject, public IInteractable {
public:
    Door(const std::string& meshPath, const glm::vec3& position, const glm::vec3& scale) : TGameObject("Door", TObjectType::OBJECT_STATIC) {
        auto& mesh = AddComponent<TStaticMeshComponent>(meshPath);
        auto& collider = AddComponent<TColliderComponent>();
        collider.Size = scale;
        GetWorldLocation() = position;
        GetWorldScale() = scale / 2.0f;
        AddTag("Interactable");
        GetWorld().GetPhysicsSystem().CreateRigidBody(this, 0.0f);
    }

    virtual void Interact() override {
        if (_State == EDoorState::OPENED)
            CloseDoor();
        else
            OpenDoor();
    }

private:
    EDoorState _State;

    void OpenDoor() {
        _State = EDoorState::OPENED;
        LOG_INFO("Door Opened");
    }
    void CloseDoor() {
        _State = EDoorState::CLOSED;
        LOG_INFO("Door Closed");
    }
};
