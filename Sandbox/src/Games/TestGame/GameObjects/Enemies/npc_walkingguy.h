#pragma once

#include "TAGE/TAGE.h"
#include "TAGE/TObjects.h"
#include "TAGE/TPhysics.h"
#include "TAGE/TMeshes.h"

#include "imgui.h"

class npc_walkingguy : public TPawn
{
public:
	npc_walkingguy() : TPawn("NPC Walking Guy") {
		SkeletalMesh->LoadModel("Assets/Models/Character/Char.glb");
	}
	~npc_walkingguy() {
	}

	void RenderBones() {
		for (auto bone : SkeletalMesh->GetSkeleton()->GetBones()) {
			ImGui::Text(bone.GetBoneName().c_str());
		}
	}

	virtual void Tick(float deltaTime) override {

	}
private:
};

REGISTER_GAMEOBJECT_CLASS(npc_walkingguy);