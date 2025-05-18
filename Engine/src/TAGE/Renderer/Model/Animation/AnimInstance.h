#pragma once

#include "TAGE/ECS/Objects/Entity.h"
#include "TAGE/Renderer/Model/Animation/Skeletal.h"
#include "TAGE/Renderer/Model/Animation/Animation.h"
#include "TAGE/ECS/Components/RenderingComponents.h"
#include "glm/glm.hpp"

namespace TAGE::RENDERER {
	class AnimInstance
	{
	public:
		AnimInstance(Entity* owner, const MEM::Ref<Model>& model);
		virtual ~AnimInstance() = default;

		virtual void Update(float dt) = 0;

		void Play(const std::string& name);
		void PlayBlend(const std::string& name, float duration);
		void LoadAnimation(const std::string& name, const std::string& path);

		void SetParameter(const std::string& name, float value);
		void SetParameter(const std::string& name, bool  value);
		void SetParameter(const std::string& name, glm::vec3 value);

		float GetFloatParam(const std::string& name) const;
		bool GetBoolParam(const std::string& name) const;
		glm::vec3 GetVec3Param(const std::string& name) const;

		void UpdateAnimInstance(float dt);

		ECS::AnimatorComponent GetAnimator() const { return _Animator; }

	protected:
		ECS::AnimatorComponent _Animator;
		std::unordered_map<std::string, MEM::Ref<Animation>> _Animations;
		std::unordered_map<std::string, float> _FloatParams;
		std::unordered_map<std::string, bool>  _BoolParams;
		std::unordered_map<std::string, glm::vec3>  _Vec3Params;
		MEM::Ref<Skeletal> _Skeletal;
		MEM::Ref<Model> _Model;
		Entity* _Owner;
	};
}