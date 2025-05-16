#pragma once

#include <glm/glm.hpp>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include "Animation.h"
#include "Bone.h"
#include "Skeletal.h"
#include "TAGE/Core/Profiler/Profiler.h"
#include <glm/gtx/matrix_decompose.hpp>

namespace TAGE::RENDERER {
	class Animator
	{
	public:
		Animator(const MEM::Ref<Skeletal>& skeletal, const MEM::Ref<Animation>& animation) : m_Skeletal(skeletal), m_CurrentTime(0.0f), m_CurrentAnimation(animation)
		{
			m_FinalBoneMatrices.reserve(100);

			for (int i = 0; i < 100; i++)
				m_FinalBoneMatrices.push_back(glm::mat4(1.0f));

		}

		void UpdateAnimation(float dt)
		{
			m_DeltaTime = dt;

			if (!m_CurrentAnimation)
				return;

			m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
			if (m_CurrentTime >= m_CurrentAnimation->GetDuration()) {
				if (m_CurrentAnimation->IsLooping()) {
					m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
				}
				else {
					m_CurrentTime = m_CurrentAnimation->GetDuration();
				}
			}

			if (m_IsBlending && m_BlendTargetAnimation)
			{
				m_BlendTime += dt;
				float blendFactor = glm::clamp(m_BlendTime / m_BlendDuration, 0.0f, 1.0f);

				glm::mat4 identity(1.0f);
				BlendBoneTransform(&m_Skeletal->m_RootNode, &m_Skeletal->m_RootNode, identity, blendFactor);

				if (blendFactor >= 1.0f)
				{
					m_CurrentAnimation = m_BlendTargetAnimation;
					m_CurrentTime = 0.0f;
					m_BlendTargetAnimation = nullptr;
					m_IsBlending = false;
				}
			}
			else
			{
				glm::mat4 identity(1.0f);
				CalculateBoneTransform(&m_Skeletal->m_RootNode, identity);
			}
		}

		void PlayAnimation(const MEM::Ref<Animation>& pAnimation)
		{
			m_CurrentAnimation = pAnimation;
			m_CurrentTime = 0.0f;
		}

		void PlayAnimationWithBlend(const MEM::Ref<Animation>& targetAnimation, float blendDuration)
		{
			if (!targetAnimation || targetAnimation == m_CurrentAnimation)
				return;

			m_BlendTargetAnimation = targetAnimation;
			m_BlendDuration = blendDuration;
			m_BlendTime = 0.0f;
			m_IsBlending = true;
		}
		MEM::Ref<Animation> GetCurrentAnimation() const { return m_CurrentAnimation; }

		void CalculateBoneTransform(const AssimpNodeData* node, const glm::mat4& parentTransform) {
			TE_PROFILE_SCOPE("calculate bone transform");
			const std::string_view nodeName = node->name;
			glm::mat4 nodeTransform = node->transformation;

			if (Bone* bone = m_Skeletal->GetBone(nodeName.data()); bone) {
				bone->Update(m_CurrentTime);
				nodeTransform = bone->GetLocalTransform();
			}

			const glm::mat4 globalTransformation = parentTransform * nodeTransform;

			const auto& boneInfoMap = m_Skeletal->GetBoneInfoMap();

			if (auto it = boneInfoMap.find(nodeName.data()); it != boneInfoMap.end()) {
				const int index = it->second.id;
				const glm::mat4& offset = it->second.offset;
				m_FinalBoneMatrices[index] = globalTransformation * offset;
			}

			for (int i = 0; i < node->childrenCount; ++i) {
				CalculateBoneTransform(&node->children[i], globalTransformation);
			}
		}

		inline glm::mat4 MixMat4(const glm::mat4& a, const glm::mat4& b, float t)
		{
			glm::vec3 transA, scaleA, transB, scaleB;
			glm::quat rotA, rotB;

			glm::vec3 skew;
			glm::vec4 perspective;

			glm::decompose(a, scaleA, rotA, transA, skew, perspective);
			glm::decompose(b, scaleB, rotB, transB, skew, perspective);

			glm::vec3 finalTrans = glm::mix(transA, transB, t);
			glm::vec3 finalScale = glm::mix(scaleA, scaleB, t);
			glm::quat finalRot = glm::slerp(rotA, rotB, t);

			glm::mat4 translationMat = glm::translate(glm::mat4(1.0f), finalTrans);
			glm::mat4 rotationMat = glm::toMat4(finalRot);
			glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), finalScale);

			return translationMat * rotationMat * scaleMat;
		}


		void BlendBoneTransform(const AssimpNodeData* fromNode, const AssimpNodeData* toNode, const glm::mat4& parentTransform, float blendFactor)
		{
			std::string nodeName = fromNode->name;
			glm::mat4 fromTransform = fromNode->transformation;
			glm::mat4 toTransform = toNode->transformation;

			if (Bone* bone = m_Skeletal->GetBone(nodeName); bone)
			{
				bone->Update(m_CurrentTime);
				fromTransform = bone->GetLocalTransform();
			}

			if (Bone* bone = m_Skeletal->GetBone(toNode->name); bone)
			{
				bone->Update(0.0f);
				toTransform = bone->GetLocalTransform();
			}

			glm::mat4 finalTransform = MixMat4(fromTransform, toTransform, blendFactor);
			glm::mat4 globalTransform = parentTransform * finalTransform;

			const auto& boneInfoMap = m_Skeletal->GetBoneInfoMap();

			if (auto it = boneInfoMap.find(nodeName); it != boneInfoMap.end())
			{
				int index = it->second.id;
				glm::mat4 offset = it->second.offset;
				m_FinalBoneMatrices[index] = globalTransform * offset;
			}

			for (int i = 0; i < fromNode->childrenCount; ++i)
			{
				BlendBoneTransform(&fromNode->children[i], &toNode->children[i], globalTransform, blendFactor);
			}
		}

		std::vector<glm::mat4> GetFinalBoneMatrices()
		{
			return m_FinalBoneMatrices;
		}

		float GetCurrentAnimationTime() const { return m_CurrentTime; }
		float GetCurrentAnimationDuration() const { return m_CurrentAnimation->GetDuration(); }
	private:
		MEM::Ref<Skeletal> m_Skeletal;
		std::vector<glm::mat4> m_FinalBoneMatrices;
		float m_CurrentTime = 0.0f;
		float m_DeltaTime = 0.0f;

		MEM::Ref<Animation> m_CurrentAnimation;
		MEM::Ref<Animation> m_BlendTargetAnimation = nullptr;
		float m_BlendDuration = 0.0f;
		float m_BlendTime = 0.0f;
		bool m_IsBlending = false;
	};
}