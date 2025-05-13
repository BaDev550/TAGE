#pragma once

#include "TAGE/TAGE.h"
#include "TAGE/TObjects.h"
#include "TAGE/TPhysics.h"
#include "TAGE/TMeshes.h"

#include "PlayerCamera.h"
#include "../Common/BasicObject.h"
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
		auto& tc = GetComponent<TTransformComponent>();
		SkeletalMesh = &AddComponent<TSkeletalMeshComponent>("Assets/Models/Arms/Arms.glb");
		auto& collider = AddComponent<TColliderComponent>();
		collider.Shape = TColliderShapeType::CAPSULE;
		collider.Size.y = 1.5f;

		tc.LocalPosition.y += 1.5f;

		GetWorld().GetPhysicsSystem().CreateRigidBody(this, 1.0f);
		idleAnim = new TEAnim("Assets/Models/Arms/Idle.glb", SkeletalMesh->model.get());
		Animator = AddComponent<TAnimatorComponent>(SkeletalMesh->GetSkeleton(), idleAnim);

		AddChild(&camera);
	}
	~Player() {
		delete idleAnim;
	}

	void DrawStats() {
		ImGui::Text("Selected Item %s", _SelectedItem.name.c_str());

		ImGui::Text("Inventory");
		for (size_t i = 0; i < _Inventory.size(); i++) {
			ImGui::Text("Item: %s", _Inventory[i].name.c_str());
		}

		ImGui::Text(IsGrounded() ? "Gounded" : "Air");

		if (ImGui::CollapsingHeader("Skeleton")) {
			for (auto& bone : SkeletalMesh->GetSkeleton()->GetBones()) {
				ImGui::Text(bone.GetBoneName().c_str());
			}
		}
	}

	void Fire() {
		auto& transform = GetComponent<TTransformComponent>();
		auto& rb = GetComponent<TRigidBodyComponent>();
		glm::vec3 start = camera.GetWorldLocation();
		glm::vec3 end = start + camera.GetForward() * 500.0f;

		TEHitResult hit = Cast::Raycast(this, start, end, true, TERayDrawType::FOR_DURATION, 5.0f);

		if (hit.hit) {
			std::cout << hit.actor->GetComponent<TTagComponent>().tag << std::endl;
		}
	}

	bool IsGrounded() {
		auto& transform = GetComponent<TTransformComponent>();
		glm::vec3 start = transform.Position;
		glm::vec3 end = start - glm::vec3(0, 1.6f, 0);

		TEHitResult hit = Cast::Raycast(this, start, end, true);
		return hit.hit;
	}

	virtual void Tick(float deltaTime) override {

        auto& camComp = camera.GetComponent<TCameraComponent>();
        glm::vec3 forward = camera.GetForward();
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
			if (_SelectedItem.type == ItemType::ITEM_WEAPON)
				Fire();
		}

		if (TEInput::IsKeyPressed(TEKey::Q)) {
			_SelectedItem = _SelectedItem.name == _Inventory[1].name ? _Inventory[0] : _Inventory[1];
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

	TEAnim* idleAnim;
	TSkeletalMeshComponent* SkeletalMesh;
	TAnimatorComponent Animator;
};

