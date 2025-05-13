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

namespace TAGE::RENDERER {
	class Animator
	{
	public:
		Animator(Skeletal* skeletal, Animation* animation) : m_Skeletal(skeletal), m_CurrentTime(0.0f), m_CurrentAnimation(animation)
		{
			m_FinalBoneMatrices.reserve(100);

			for (int i = 0; i < 100; i++)
				m_FinalBoneMatrices.push_back(glm::mat4(1.0f));

		}

		void UpdateAnimation(float dt)
		{
			m_DeltaTime = dt;
			if (m_CurrentAnimation)
			{
				m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
				if (m_CurrentTime >= m_CurrentAnimation->GetDuration())
				{
					m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
				}

				glm::mat4 rootTransformation = glm::mat4(1.0f);
				CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), rootTransformation);
			}

		}

		void PlayAnimation(Animation* pAnimation)
		{
			m_CurrentAnimation = pAnimation;
			m_CurrentTime = 0.0f;
		}
		Animation* GetCurrentAnimation() const { return m_CurrentAnimation; }

		void CalculateBoneTransform(const AssimpNodeData* node, const glm::mat4& parentTransform) {
			TE_PROFILE_SCOPE("calculate bone transform");
			const std::string_view nodeName = node->name;
			glm::mat4 nodeTransform = node->transformation;

			if (Bone* bone = m_Skeletal->GetBone(nodeName.data()); bone) {
				bone->Update(m_CurrentTime);
				nodeTransform = bone->GetLocalTransform();
			}

			const glm::mat4 globalTransformation = parentTransform * nodeTransform;

			const auto& boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
			if (auto it = boneInfoMap.find(nodeName.data()); it != boneInfoMap.end()) {
				const int index = it->second.id;
				const glm::mat4& offset = it->second.offset;
				m_FinalBoneMatrices[index] = globalTransformation * offset;
			}

			for (int i = 0; i < node->childrenCount; ++i) {
				CalculateBoneTransform(&node->children[i], globalTransformation);
			}
		}

		std::vector<glm::mat4> GetFinalBoneMatrices()
		{
			return m_FinalBoneMatrices;
		}

		float GetCurrentAnimationTime() const { return m_CurrentTime; }
	private:
		Skeletal* m_Skeletal;
		std::vector<glm::mat4> m_FinalBoneMatrices;
		Animation* m_CurrentAnimation;
		float m_CurrentTime = 0.0f;
		float m_DeltaTime = 0.0f;

	};
}