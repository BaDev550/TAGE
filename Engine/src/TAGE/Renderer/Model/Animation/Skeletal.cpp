#include "tagepch.h"
#include "Skeletal.h"
#include "TAGE/Renderer/Model/Model.h"
#include "TAGE/Renderer/Model/Animation/Animation.h"

namespace TAGE::RENDERER {
    void Skeletal::SetBoneParent(const std::string& childName, const std::string& parentName) {
        Bone* child = GetBone(childName);
        Bone* parent = GetBone(parentName);
        if (child && parent)
            child->SetParentBone(parent);
    }
    void Skeletal::AddSocketToBone(const std::string& boneName, Socket& socket) {
        for (auto& bone : m_Bones) {
            if (bone.GetBoneName() == boneName) {
                bone.AddSocket(socket);
                return;
            }
        }
        CORE_LOG_WARN("Bone with name {} not found for socket attachment.", boneName);
    }
    Socket* Skeletal::GetSocket(const std::string& socketName) {
        for (auto& bone : m_Bones) {
            Socket* socket = bone.GetSocket(socketName);
            if (socket) return socket;
        }
        return nullptr;
    }
    Bone* Skeletal::GetBone(const std::string& name) {
        for (auto& bone : m_Bones) {
            if (bone.GetBoneName() == name) {
                return &bone;
            }
        }
        return nullptr;
    }
    void Skeletal::Update(float animationTime) {
        for (auto& bone : m_Bones) {
            bone.Update(animationTime);
        }
    }

    void Skeletal::LoadBonesFromMesh(const aiScene* scene)
    {
        int boneCount = 0;
        for (unsigned int i = 0; i < scene->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[i];
            for (unsigned int j = 0; j < mesh->mNumBones; j++) {
                std::string boneName = mesh->mBones[j]->mName.C_Str();

                if (m_BoneInfoMap.find(boneName) == m_BoneInfoMap.end()) {
                    m_BoneInfoMap[boneName].id = boneCount++;
                    m_BoneInfoMap[boneName].offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[j]->mOffsetMatrix);
                }

                AddBone(Bone(boneName, m_BoneInfoMap[boneName].id));
            }
        }
    }

    void Skeletal::ReadHierarchyData(AssimpNodeData& dest, const aiNode* src) {
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

    void Skeletal::BuildBoneHierarchy(const AssimpNodeData& node, const std::string& parentName) {
        if (!parentName.empty()) {
            SetBoneParent(node.name, parentName);
        }

        for (const auto& child : node.children) {
            BuildBoneHierarchy(child, node.name);
        }
    }
}