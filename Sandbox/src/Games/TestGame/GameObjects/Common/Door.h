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
    Door() : TGameObject("Door", TObjectType::OBJECT_STATIC) {}

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
        float startZ = GetLocalLocation().z;
        float targetZ = 1.5f;

        TETimeLine::Play(startZ, targetZ, 1.0f, [this](float value) {
            GetLocalLocation().z = value;
        }, [this]() {
            LOG_INFO("Door fully opened.");
            _State = EDoorState::OPENED;
        });
    }
    void CloseDoor() {
        float startZ = GetLocalLocation().z;
        float targetZ = 0.0f;

        TAGE::Timeline::Play(startZ, targetZ, 1.0f, [this](float value) {
            GetLocalLocation().z = value;
        }, [this]() {
            LOG_INFO("Door fully closed.");
            _State = EDoorState::CLOSED;
        });
    }
};

REGISTER_GAMEOBJECT_CLASS(Door);