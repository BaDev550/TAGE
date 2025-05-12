#include "tagepch.h"
#include "Animation.h"

namespace TAGE::RENDERER {
	Animation::Animation(const std::string& animationPath, Model* model)
	{
		m_Skeletal = model->GetSkeletal();
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(animationPath,
			aiProcess_Triangulate
		);
		ENGINE_ASSERT(scene && scene->mRootNode, "Failed to load animation: {}", animationPath);
		auto animation = scene->mAnimations[0];
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		aiMatrix4x4 globalTransformation = scene->mRootNode->mTransformation;
		globalTransformation = globalTransformation.Inverse();
		ReadHierarchyData(m_RootNode, scene->mRootNode);
		CORE_LOG_INFO("Root Node name: {}", m_RootNode.name);
		ReadMissingBones(animation, *model);
	}
}