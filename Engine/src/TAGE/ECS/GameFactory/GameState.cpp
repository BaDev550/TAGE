#include "tagepch.h"
#include "GameState.h"
#include "TAGE/ECS/Objects/Pawn.h"

namespace TAGE {
	void GameState::Init(World* world)
	{
		_World = world;
	}

	void GameState::Tick(float dt)
	{
		for (auto& player : _Players)
		{
			player->Tick(dt);

			if (auto pawn = player->GetPawn())
			{
				if (auto* p = dynamic_cast<Pawn*>(pawn))
				{
					p->Tick(dt);
				}
			}
		}
	}
}