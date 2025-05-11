#pragma once

#include "TAGE/TAGE.h"
#include "PlayerCamera.h"
#include "imgui.h"

enum class ItemType {
	ITEM_WEAPON,
	ITEM_FOOD
};
struct Item {
	std::string name;
	ItemType type;
};

class Player : public TGameObject
{
public:
	Player() : TGameObject("Player", TObjectType::OBJECT_CHARACTER) {
		AddComponent<TSkeletalMeshComponent>("Assets/Models/Arms/Arms.glb");
		AddComponent<TColliderComponent>();
		GetWorld().GetPhysicsSystem().CreateRigidBody(this, 1.0f);
		AddChild(&camera);
	}

	void DrawStats() {
		ImGui::Text("Selected Item %s", _SelectedItem.name);

		ImGui::Text("Inventory");
		for (size_t i = 0; i < _Inventory.size(); i++) {
			ImGui::Text("Item: %s", _Inventory[i].name);
		}
	}

	virtual void Tick(float deltaTime) override {

		if (TEInput::IsKeyPressed(TEKey::E)) {
			if (_SelectedItem.type == ItemType::ITEM_WEAPON) {
				auto& activeCam = camera.GetComponent<TCameraComponent>();

				glm::vec2 mousePos = TEInput::GetMousePosition();

				glm::vec3 rayOrigin = activeCam.Camera->GetPosition();
				glm::vec3 rayEnd = rayOrigin + camera.GetCamera().Camera->GetFront() * 1000.0f;

				TEHitResult hit = TECast::Raycast(rayOrigin, rayEnd, TERayDrawType::FOR_DURATION, 5.0f);

				if (hit.hit && hit.actor && hit.actor->HasComponent<TTagComponent>()) {

				}
			}
		}
		if (TEInput::IsKeyPressed(TEKey::X)) {
			_SelectedItem = _Inventory[0];
		}
		if (TEInput::IsKeyPressed(TEKey::C)) {
			_SelectedItem = _Inventory[1];
		}
	};
private:
	PlayerCamera camera;
	std::vector<Item> _Inventory = {
		{ "Test", ItemType::ITEM_FOOD },
		{ "AK-47", ItemType::ITEM_WEAPON },
		{ "Aug", ItemType::ITEM_WEAPON },
	};
	Item _SelectedItem;
};

