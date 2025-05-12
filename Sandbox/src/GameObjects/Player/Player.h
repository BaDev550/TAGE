#pragma once

#include "TAGE/TAGE.h"
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
		walkAnim = new TEAnim("Assets/Models/Arms/Walk.glb", SkeletalMesh->model.get());
		Animator = AddComponent<TAnimatorComponent>(SkeletalMesh->GetSkeleton(), idleAnim);

		TESocket socket("WeaponSocket", 0);
		SkeletalMesh->GetSkeleton()->AddSocketToBone("mixamorig1:RightHand", socket);
		for (auto& bone : SkeletalMesh->GetSkeleton()->GetBones())
			LOG_INFO("Bone Name: {}", bone.GetBoneName());

		AddChild(&camera);
	}
	~Player() {
		delete idleAnim;
		delete walkAnim;
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
		glm::vec3 end = start - glm::vec3(0, 1.6f, 0);

		TEHitResult hit = Raycast(this, start, end, true);
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

		if (glm::length(rb.GetVelocity()) > 10.0f) {
			if (Animator.AnimatorInstance->GetCurrentAnimation() != walkAnim)
				Animator.AnimatorInstance->PlayAnimation(walkAnim);
		}
		else {
			if (Animator.AnimatorInstance->GetCurrentAnimation() != idleAnim)
				Animator.AnimatorInstance->PlayAnimation(idleAnim);
		}

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

		TESocket* socket = SkeletalMesh->GetSkeleton()->GetSocket("WeaponSocket");
		glm::mat4 weaponTransform;
		if (socket) {
			weaponTransform = socket->GetLocalTransform();
			LOG_WARN("Weapon Socket Local Transform: {}", glm::to_string(weaponTransform));
			LOG_WARN("Weapon Socket World Position: {}", glm::to_string(socket->GetWorldPosition()));
			object.GetTransform().Position = socket->GetWorldPosition();
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
	TEAnim* walkAnim;
	BasicObject object;
	TSkeletalMeshComponent* SkeletalMesh;
	TAnimatorComponent Animator;
};

