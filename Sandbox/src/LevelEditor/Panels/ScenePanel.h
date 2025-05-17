#pragma once
#include "imgui.h"
#include "TAGE/TAGE.h"
#include "TAGE/TObjects.h"
#include "TAGE/TComponents.h"
#include "TAGE/TMeshes.h"

#include "../TeGui.h"
#include "Panel.h"

class ScenePanel : public Panel {
public:
	virtual void Begin() override {
		ImGui::Begin("Scene Hierarchy");
		auto actors = GetApp().GetScene().GetWorld().GetAllActors();

		if (ImGui::Button("Create Actor")) {
			std::string name = "Actor (" + std::to_string(actors.size()) + ")";
			TActor* tmp_actor = GetApp().GetScene().GetWorld().SpawnActor(name);
		}

		for (auto actor : actors) {
			if (!actor || !actor->IsValid()) continue;

			if (ImGui::Selectable(actor->GetComponent<TTagComponent>().name.c_str())) {
				_SelectedObject = actor;
			}
		}

		ImGui::End();

		Propeties();
	}

	void SetSelectedActor(TActor* SelectedActor) { _SelectedObject = SelectedActor; }
	TActor* GetSelectedActor() const { return _SelectedObject; }
private:
	void Propeties() {
		ImGui::Begin("Property Inspector");

		if (_SelectedObject) {
			auto& tag = _SelectedObject->GetComponent<TTagComponent>().name;
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, tag.c_str());
			if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
				tag = std::string(buffer);

			auto& transform = _SelectedObject->GetComponent<TTransformComponent>();
			TeGui::DrawVec3Control("Position", transform.Position);
			TeGui::DrawVec3Control("Rotation", transform.Rotation);
			TeGui::DrawVec3Control("Scale", transform.Scale, 1.0f);

			if (_SelectedObject->HasComponent<TColliderComponent>()) {
				auto& collider = _SelectedObject->GetComponent<TColliderComponent>();

				std::vector<std::string> colliderTypes = { "Box", "Sphere", "Capsule" };
				std::vector<std::string> colliderRespondes = { "None", "Block", "Overlap" };
				int selectedCollider = static_cast<int>(collider.Shape);
				int selectedResponde = static_cast<int>(collider.ResponseType);
				if (TeGui::ComboBox("Collider Type", colliderTypes, selectedCollider)) {
					collider.Shape = static_cast<TAGE::ECS::ColliderShapeType>(selectedCollider);
					collider.Dirty = true;
				}

				if (TeGui::ComboBox("Collider Respounde", colliderRespondes, selectedResponde)) {
					collider.ResponseType = static_cast<TAGE::ECS::CollisionResponseType>(selectedResponde);
					collider.Dirty = true;
				}

				if (TeGui::DrawVec3Control("Collider Size", collider.Size)) {
					collider.Dirty = true;
				}
			}

			if (_SelectedObject->HasComponent<TRigidBodyComponent>()) {
				auto& rb = _SelectedObject->GetComponent<TRigidBodyComponent>();
				std::vector<std::string> types = { "Static", "Dynamic", "Kinematic" };
				int currentType = static_cast<int>(rb.BodyType);

				if (TeGui::ComboBox("Body Type", types, currentType)) {
					rb.BodyType = static_cast<TAGE::ECS::PhysicsBodyType>(currentType);

					int flags = rb.Body->getCollisionFlags();
					flags &= ~(btCollisionObject::CF_KINEMATIC_OBJECT);

					switch (rb.BodyType) {
					case TAGE::ECS::PhysicsBodyType::STATIC:
						rb.Body->setMassProps(0.0f, btVector3(0, 0, 0));
						rb.Body->setActivationState(ISLAND_SLEEPING);
						break;
					case TAGE::ECS::PhysicsBodyType::DYNAMIC:
						rb.Body->setMassProps(1.0f, btVector3(1, 1, 1));
						rb.Body->setActivationState(ACTIVE_TAG);
						break;
					case TAGE::ECS::PhysicsBodyType::KINEMATIC:
						flags |= btCollisionObject::CF_KINEMATIC_OBJECT;
						rb.Body->setActivationState(DISABLE_DEACTIVATION);
						break;
					}

					rb.Body->setCollisionFlags(flags);
				}
			}

			if (_SelectedObject->HasComponent<TStaticMeshComponent>()) {
				if (ImGui::Button("Load Model"))
					_WantsToLoadModel = true;
			}

			if (_SelectedObject->HasComponent<TSkeletalMeshComponent>()) {
				if (ImGui::Button("Load Model"))
					_WantsToLoadModel = true;

				auto skeletal = _SelectedObject->GetComponent<TSkeletalMeshComponent>();

				if (skeletal.GetSkeleton()) {
					if (ImGui::CollapsingHeader("Skeleton")) {
						for (auto bone : skeletal.GetSkeleton()->GetBones()) {
							ImGui::Selectable(bone.GetBoneName().c_str());
						}
					}
				}
			}

			std::vector<std::string> components = { "None", "RigidBody", "Collider", "StaticMesh", "SkeletalMesh", "EditorCamera" };

			if (TeGui::ComboBox("Add Component", components, _SelectedComponent)) {
				if (_SelectedComponent == 1 && !_SelectedObject->HasComponent<TRigidBodyComponent>())
					GetApp().GetPhysicsSystem().CreateRigidBody(_SelectedObject, 1.0f);

				if (_SelectedComponent == 2 && !_SelectedObject->HasComponent<TColliderComponent>())
					_SelectedObject->AddComponent<TColliderComponent>();

				if (_SelectedComponent == 3 && !_SelectedObject->HasComponent<TStaticMeshComponent>() && !_SelectedObject->HasComponent<TSkeletalMeshComponent>())
					_SelectedObject->AddComponent<TStaticMeshComponent>();

				if (_SelectedComponent == 4 && !_SelectedObject->HasComponent<TSkeletalMeshComponent>() && !_SelectedObject->HasComponent<TStaticMeshComponent>())
					_SelectedObject->AddComponent<TSkeletalMeshComponent>();

				if (_SelectedComponent == 5 && !_SelectedObject->HasComponent<TCameraComponent>())
					_SelectedObject->AddComponent<TCameraComponent>(TCamType::EDITOR);

				_SelectedComponent = 0;
			}
		}

		ImGui::End();

		if (_WantsToLoadModel) {
			ImGuiFileDialog::Instance()->OpenDialog("ChooseModel", "Choose Model File", ".fbx,.obj,.gltf,.glb");
			_WantsToLoadModel = false;
		}

		if (ImGuiFileDialog::Instance()->Display("ChooseModel")) {
			if (ImGuiFileDialog::Instance()->IsOk()) {
				std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
				std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();

				if (_SelectedObject) {
					if (_SelectedObject->HasComponent<TStaticMeshComponent>()) {
						auto& mesh = _SelectedObject->GetComponent<TStaticMeshComponent>();
						mesh.LoadModel(filePath);
					}

					if (_SelectedObject->HasComponent<TSkeletalMeshComponent>()) {
						auto& skeletal = _SelectedObject->GetComponent<TSkeletalMeshComponent>();
						skeletal.LoadModel(filePath);
					}
				}
			}

			ImGuiFileDialog::Instance()->Close();
		}

	}
private:
	TActor* _SelectedObject = nullptr;
	int _SelectedComponent = 0;
	bool _WantsToLoadModel = false;
};