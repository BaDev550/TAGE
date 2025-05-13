#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include <functional>
#include "AnimData.h"
#include "Bone.h"
#include "TAGE/Renderer/Model/Model.h"
#include "TAGE/Renderer/Model/Animation/Skeletal.h"

namespace TAGE::RENDERER {
	struct AssimpNodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<AssimpNodeData> children;
	};

	class Animation
	{
	public:
		Animation() = default;
		Animation(const std::string& animationPath, Model* model);
		~Animation() {
		}

		Bone* FindBone(const std::string& name) {
			return m_Skeletal->GetBone(name);
		}
		void SetLoop(bool loop) { m_Loop = loop; }

		inline float GetTicksPerSecond() { return m_TicksPerSecond * 0.5f; }
		inline float GetDuration() { return m_Duration; }
		inline const AssimpNodeData& GetRootNode() { return m_RootNode; }
		inline const std::map<std::string, BoneInfo>& GetBoneIDMap() { return m_BoneInfoMap; }
		inline bool IsLooping() { return m_Loop; }
	private:
		void ReadMissingBones(const aiAnimation* animation, Model& model) {
			int size = animation->mNumChannels;
			auto& boneInfoMap = model.GetBoneInfoMap();
			int& boneCount = model.GetBoneCount();

			for (int i = 0; i < size; i++) {
				auto channel = animation->mChannels[i];
				std::string boneName = channel->mNodeName.C_Str();

				if (boneInfoMap.find(boneName) == boneInfoMap.end()) {
					boneInfoMap[boneName].id = boneCount;
					boneCount++;
				}
				m_Skeletal->AddBone(Bone(channel->mNodeName.C_Str(),
					boneInfoMap[channel->mNodeName.C_Str()].id, channel));
			}

			m_BoneInfoMap = boneInfoMap;
		}

		void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src) {
			ENGINE_ASSERT(src, "Node is empty");

			dest.name = src->mName.data;
			dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
			dest.childrenCount = src->mNumChildren;
			dest.children.reserve(src->mNumChildren);

			for (unsigned int i = 0; i < src->mNumChildren; i++) {
				AssimpNodeData childData;
				ReadHierarchyData(childData, src->mChildren[i]);
				dest.children.push_back(childData);
			}
		}

		void BuildBoneHierarchy(const AssimpNodeData& node, Skeletal* skeletal, const std::string& parentName = "") {
			if (!parentName.empty()) {
				skeletal->SetBoneParent(node.name, parentName);
			}

			for (const auto& child : node.children) {
				BuildBoneHierarchy(child, skeletal, node.name);
			}
		}

		float m_Duration = 0.0f;
		int m_TicksPerSecond;
		Skeletal* m_Skeletal;
		bool m_Loop = false;
		AssimpNodeData m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
	};
}