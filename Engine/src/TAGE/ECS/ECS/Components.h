#pragma once

#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "TAGE/Renderer/Model/Model.h"
#include "TAGE/Renderer/Model/Animation/Animator.h"
#include "TAGE/Renderer/Shader/ShaderLibrary.h"
#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Renderer/Camera.h"
#include <vector>

namespace TAGE::ECS {
	using namespace RENDERER;

	struct TransformComponent {
	public:
		glm::vec3 Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

		glm::mat4 GetMatrix() const {
			glm::mat4 translation = glm::translate(glm::mat4(1.0f), Position);
			glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1, 0, 0));
			glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0, 1, 0));
			glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0, 0, 1));
			glm::mat4 rotation = rotZ * rotY * rotX;
			glm::mat4 scale = glm::scale(glm::mat4(1.0f), Scale);
			return translation * rotation * scale;
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

		MEM::Scope<Model> model;
		EMeshType meshType = EMeshType::STATIC;
		bool IsVisible = true;

		void Draw(MEM::Ref<Shader> shader, const glm::mat4& transform) {
			if (IsVisible)
				model->Draw(transform, shader);
		}

		void LoadModel(const std::string& modelPath) {
			model->Reset();
			model.reset(new Model(modelPath));
		}
	};

	struct StaticMeshComponent : public MeshComponent
	{
		StaticMeshComponent(const std::string& path)
			: MeshComponent(path, EMeshType::STATIC) {
		}
	};

	struct SkeletalMeshComponent : public MeshComponent
	{
		SkeletalMeshComponent(const std::string& path)
			: MeshComponent(path, EMeshType::SKELETAL) {
		}
	};

	struct AnimatorComponent
	{
		MEM::Scope<Animator> AnimatorInstance;

		AnimatorComponent() = default;

		AnimatorComponent(Animation* animation)
		{
			AnimatorInstance = MEM::CreateScope<Animator>(animation);
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
		glm::mat4& GetViewMatrix() const { Camera->GetViewMatrix(); }
		glm::mat4& GetProjectionMatrix() const { Camera->GetProjectionMatrix(); }
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
}