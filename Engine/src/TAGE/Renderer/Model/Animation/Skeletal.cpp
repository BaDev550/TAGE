#include "tagepch.h"
#include "Skeletal.h"
#include "TAGE/Renderer/Model/Model.h"

namespace TAGE::RENDERER {
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
}