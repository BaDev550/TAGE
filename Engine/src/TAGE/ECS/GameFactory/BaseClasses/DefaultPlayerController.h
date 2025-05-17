#pragma once

#include "TAGE/ECS/GameFactory/PlayerController.h"

namespace TAGE {
    class DefaultPlayerController : public PlayerController {
    public:
        virtual void Init(World* world) override {
            _World = world;
        }

        virtual void Tick(float dt) override {
        }
    };
}