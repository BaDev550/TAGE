#pragma once

#include "TAGE/TAGE.h"

class PlayerCamera : public TGameObject
{
public:
	PlayerCamera() : TGameObject("Player Camera", TObjectType::OBJECT_CAMERA) {
		AddComponent<TStaticMeshComponent>("Assets/Models/Cube/cube.obj");
		Component = &AddComponent<TCameraComponent>(TCamType::Perfective);
	}

	inline TCameraComponent& GetCamera() const { return *Component; }
private:
	TCameraComponent* Component;
};