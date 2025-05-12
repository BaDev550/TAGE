#pragma once

#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <assimp/scene.h>
#include "Bone.h"
#include "Socket.h"

namespace TAGE::RENDERER {
	class Model;
	class Skeletal
	{
	public:
		Skeletal(Model* model) : _Model(model) {}

        void AddBone(const Bone& bone) { m_Bones.push_back(bone); }
        void AddSocketToBone(const std::string& boneName, Socket& socket);
        Socket* GetSocket(const std::string& socketName);
        Bone* GetBone(const std::string& name);
        void Update(float animationTime);

        const std::vector<Bone>& GetBones() const { return m_Bones; }
	private:
		Model* _Model;
		std::vector<Bone> m_Bones;
	};
}