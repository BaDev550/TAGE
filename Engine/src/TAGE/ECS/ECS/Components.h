#pragma once

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include "TAGE/Renderer/Model/Model.h"
#include "TAGE/Renderer/Model/Animation/Animator.h"
#include "TAGE/Renderer/Shader/ShaderLibrary.h"
#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Renderer/Camera.h"
#include <btBulletDynamicsCommon.h>
#include "TAGE/ECS/Objects/Actor.h"
#include <vector>

namespace TAGE::ECS {
	using namespace RENDERER;

	struct TransformComponent {
	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::vec3 LocalPosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 LocalRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 LocalScale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 WorldMatrix = glm::mat4(1.0f);

		glm::mat4 GetMatrix() const {
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), (LocalPosition + Position));
			glm::mat4 rotation = glm::eulerAngleYXZ(
				glm::radians((LocalRotation.x + Rotation.y)),
				glm::radians((LocalRotation.y + Rotation.x)),
				glm::radians((LocalRotation.z + Rotation.z))
			);
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), (LocalScale * Scale));
			return translation * rotation * scale;
		}

		void UpdateWorldMatrix(const glm::mat4& parentWorldMatrix) {
			WorldMatrix = parentWorldMatrix * GetMatrix();
		}
	};

	enum class ColliderShapeType { BOX, SPHERE, CAPSULE, MESH};
	enum class CollisionResponseType { NONE, OVERLAP, BLOCK  };
	struct ColliderComponent {
		ColliderShapeType Shape = ColliderShapeType::BOX;
		CollisionResponseType ResponseType = CollisionResponseType::BLOCK;
		glm::vec3 Size = glm::vec3(1.0f);
		bool Dirty = false;

		bool IsTrigger() const { return ResponseType == CollisionResponseType::OVERLAP; }
	};

	enum class PhysicsBodyType { Static = 0, Dynamic, Kinematic };
	struct RigidBodyComponent {
		btRigidBody* Body = nullptr;
		btDefaultMotionState* MotionState = nullptr;
		btCollisionShape* CollisionShape = nullptr;
		PhysicsBodyType BodyType = PhysicsBodyType::Dynamic;
		Actor* Owner;

		RigidBodyComponent(btRigidBody* body, btDefaultMotionState* motion, btCollisionShape* shape, Actor* owner)
			: Body(body), MotionState(motion), CollisionShape(shape), Owner(owner) {
			body->setUserPointer(Owner);
		}

		~RigidBodyComponent() {
		}

		void AddForce(const glm::vec3& force) {
			Body->activate();
			Body->applyCentralForce(btVector3(force.x, force.y, force.z));
		}

		void SetVelocity(const glm::vec3& vel) {
			Body->activate();
			Body->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
		}

		glm::vec3& GetVelocity() const {
			return glm::vec3(Body->getLinearVelocity().getX(), Body->getLinearVelocity().getY(), Body->getLinearVelocity().getZ());
		}
	};

	struct TagComponent
	{
	public:
		std::string tag;
		TagComponent(const std::string _tag) : tag(_tag) {}
		TagComponent() {}
	};

	struct MeshComponent
	{
	public:
		MeshComponent(const std::string& modelPath, EMeshType type = EMeshType::STATIC) {
			meshType = type;
			model = MEM::CreateScope<Model>(modelPath, meshType);
		}
		MeshComponent(EMeshType type = EMeshType::STATIC) { meshType = type; model = MEM::CreateScope<Model>(type); }

		MEM::Scope<Model> model;
		EMeshType meshType = EMeshType::STATIC;
		bool IsVisible = true;

		void Draw(MEM::Ref<Shader> shader, const glm::mat4& transform) {
			if (IsVisible) {
				shader->Bind();
				shader->SetUniform("u_SkeletalMesh", meshType == EMeshType::SKELETAL);
				model->Draw(transform, shader);
			}
		}

		void LoadModel(const std::string& modelPath) {
			model->Reset();
			model.reset(new Model(modelPath, meshType));
		}
	};

	struct StaticMeshComponent : public MeshComponent
	{
		StaticMeshComponent(const std::string& path)
			: MeshComponent(path, EMeshType::STATIC) {
		}
		StaticMeshComponent() : MeshComponent(EMeshType::STATIC) {}
	};

	struct SkeletalMeshComponent : public MeshComponent
	{
		SkeletalMeshComponent(const std::string& path)
			: MeshComponent(path, EMeshType::SKELETAL) {
		}
		SkeletalMeshComponent() : MeshComponent(EMeshType::SKELETAL) {}

		Skeletal* GetSkeleton() const { return model->GetSkeletal(); }
	};

	struct AnimatorComponent
	{
		MEM::Ref<Animator> AnimatorInstance;

		AnimatorComponent() = default;

		AnimatorComponent(Skeletal* skeleton, Animation* animation)
		{
			AnimatorInstance = MEM::CreateRef<Animator>(skeleton, animation);
		}
	};

	enum class ECameraType {
		Editor,
		Perfective
	};
	struct CameraComponent {
	public:
		CameraComponent(ECameraType type) {
			switch (type)
			{
			case ECameraType::Editor: Camera = MEM::CreateRef<EditorCamera>(45.0f, 1.0f, 0.1f, 100.0f); break;
			case ECameraType::Perfective: Camera = MEM::CreateRef<PerspectiveCamera>(45.0f, 1.0f, 0.1f, 100.0f); break;
			}
		}

		MEM::Ref<Camera> Camera;
		glm::mat4 GetViewMatrix() const { return Camera->GetViewMatrix(); }
		glm::mat4 GetProjectionMatrix() const { return Camera->GetProjectionMatrix(); }
		glm::vec3 GetForward() { return Camera->GetFront(); }
	private:
	};

	struct EditorCameraComponent : public CameraComponent {
	public:
		EditorCameraComponent() : CameraComponent(ECameraType::Editor) {}
	};

	struct LightComponent {
		glm::vec4 color;
		float intensity;
	};

	struct DirectionalLightComponent : public LightComponent {
		glm::vec3 direction;
	};

	struct PointLightComponent : public LightComponent {
		glm::vec3 direction;
	};

	struct RelationshipComponent {
		entt::entity Parent = entt::null;
		std::vector<entt::entity> Children;
	};
}