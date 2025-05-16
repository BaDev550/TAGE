#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "Bone.h"
#include "AnimData.h"
#include "Socket.h"

namespace TAGE::RENDERER {
	class Model;
	class Skeletal
	{
	public:
		Skeletal(Model* model) : _Model(model) {}

        void AddBone(const Bone& bone) { m_Bones.push_back(bone); }
		void SetBoneParent(const std::string& childName, const std::string& parentName);
        void AddSocketToBone(const std::string& boneName, Socket& socket);
        Socket* GetSocket(const std::string& socketName);
        Bone* GetBone(const std::string& name);
        void Update(float animationTime);

        const std::vector<Bone>& GetBones() const { return m_Bones; }
		const std::map<std::string, BoneInfo>& GetBoneInfoMap() const { return m_BoneInfoMap; }
		std::map<std::string, BoneInfo>& GetBoneInfoMap() { return m_BoneInfoMap; }

        void LoadBonesFromMesh(const aiScene* scene);
        void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src);
        void BuildBoneHierarchy(const AssimpNodeData& node, const std::string& parentName = "");

		AssimpNodeData m_RootNode;
	private:
		Model* _Model;
		std::vector<Bone> m_Bones;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
		int m_BoneCounter = 0;
	};
}