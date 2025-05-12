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
		auto& collider = AddComponent<TColliderComponent>();
		collider.Shape = TColliderShapeType::CAPSULE;
		GetWorld().GetPhysicsSystem().CreateRigidBody(this, 1.0f);
		AddChild(&camera);
	}

	void DrawStats() {
		ImGui::Text("Selected Item %s", _SelectedItem.name.c_str());

		ImGui::Text("Inventory");
		for (size_t i = 0; i < _Inventory.size(); i++) {
			ImGui::Text("Item: %s", _Inventory[i].name.c_str());
		}

		ImGui::Text(IsGrounded() ? "Gounded" : "Air");
	}

	void Fire() {
		auto& transform = GetComponent<TTransformComponent>();
		auto& rb = GetComponent<TRigidBodyComponent>();
		glm::vec3 start = camera.GetWorldLocation();
		glm::vec3 end = start + camera.GetForward() * 500.0f;

		TEHitResult hit = Raycast(this, start, end, true, TERayDrawType::FOR_DURATION, 5.0f);

		if (hit.hit) {
			std::cout << hit.actor->GetComponent<TTagComponent>().tag << std::endl;
		}
	}

	bool IsGrounded() {
		auto& transform = GetComponent<TTransformComponent>();
		glm::vec3 start = transform.Position;
		glm::vec3 end = start - glm::vec3(0, 1.0f, 0);

		TEHitResult hit = Raycast(this, start, end, true);
		return hit.hit;
	}

	virtual void Tick(float deltaTime) override {

        auto& camComp = camera.GetComponent<TCameraComponent>();
        glm::vec3 forward = camComp.Camera->GetFront();
		glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0, 1, 0)));

        glm::vec3 moveDirection(0.0f);
        float moveSpeed = 500.0f;

        if (TEInput::IsKeyPressed(TEKey::W)) moveDirection += forward;
        if (TEInput::IsKeyPressed(TEKey::S)) moveDirection -= forward;
        if (TEInput::IsKeyPressed(TEKey::A)) moveDirection -= right;
        if (TEInput::IsKeyPressed(TEKey::D)) moveDirection += right;

        if (glm::length(moveDirection) > 0.1f)
            moveDirection = glm::normalize(moveDirection) * moveSpeed * deltaTime;

        auto& transform = GetComponent<TTransformComponent>();
		auto& rb = GetComponent<TRigidBodyComponent>();
		rb.AddForce(moveDirection);

		if (TEInput::IsKeyPressed(TEKey::Space) && IsGrounded()) {
			rb.AddForce(glm::vec3(0.0f, 100.0f, 0.0f));
		}

		if (TEInput::IsMouseButtonPressed(TEMouseButton::Left)) {
			Fire();
		}

		camera.ProcessCamera(transform);
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

