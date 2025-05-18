#include "tagepch.h"
#include "PlayerController.h"

namespace TAGE {
	void PlayerController::Init(World* world)
	{
		_World = world;
		_InputComponent = MEM::CreateRef<ECS::InputComponent>();
	}

	void PlayerController::Tick(float dt)
	{
		_InputComponent->Update();
	}
}