#pragma once

#include "TAGE/ECS/GameFactory/GameMode.h"
#include "TAGE/ECS/GameFactory/GameState.h"
#include "TAGE/ECS/GameFactory/PlayerController.h"
#include "DefaultPlayerController.h"

namespace TAGE {
    class DefaultGameMode : public GameMode {
    public:
        virtual void Init(World* world) override {
            GameMode::Init(world);

            auto playerController = MEM::CreateRef<DefaultPlayerController>();
            playerController->Init(world);

            _GameState->AddPlayer(playerController);
            OnPlayerJoined(playerController);
        }

        virtual void OnPlayerJoined(MEM::Ref<PlayerController> controller) override {
            auto pawnEntity = _World->GetScene()->CreateEntity("MyPawn");
            controller->Possess(pawnEntity);
        }
    };
}