#pragma once

#include "TAGE/Core/AssetManager/Asset.h"
#include "TAGE/Core/AssetManager/AssetManager.h"
#include "TAGE/Renderer/Buffers/Buffers.h"
#include "TAGE/Renderer/Buffers/VertexArrayBuffer.h"
#include "TAGE/Renderer/Model/Animation/Socket.h"
#include "TAGE/Renderer/Material/Material.h"
#include "TAGE/Renderer/Renderer.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "glm/gtc/type_ptr.hpp"
#include "AssimpGLMHelpers.h"
#include "Animation/AnimData.h"
#include "Animation/Skeletal.h"
#include <cfloat>

namespace TAGE::RENDERER {
	struct Mesh {
		MEM::Ref<VertexArrayBuffer> VAO;
		MEM::Ref<Material> MaterialR;
		int IndexCount;
		float Radius;
	};

	enum class EMeshType {
		STATIC,
		SKELETAL
	};

	class Model : public ASSET::Asset
	{
	public:
		Model(const std::string& path, EMeshType type = EMeshType::STATIC);
		Model(EMeshType type = EMeshType::STATIC) {}
		~Model() = default;

		void Draw(glm::mat4 transform, const MEM::Ref<RENDERER::Shader>& shader) const;
		virtual ASSET::AssetType GetType() const override { return ASSET::AssetType::MODEL; }

		auto& GetBoneInfoMap() { return m_BoneInfoMap; }
		int& GetBoneCount() { return m_BoneCounter; }

		void Reset();
		void LoadModel(const std::string& path);

		MEM::Ref<Skeletal> GetSkeletal() const { return _Skeletal; }
		glm::vec3 GetCenterOffset() const { return _CenterOffset; }
		float GetBoundingRadius() const { return _BoundingRadius; }
		std::string GetPath() const { return _Path; };
	private:
		std::vector<Mesh> _Meshes;
		std::string _Directory;
		std::string _Path;
		MEM::Ref<Skeletal> _Skeletal;
		EMeshType _MeshType;
		glm::vec3 _CenterOffset;

		float _BoundingRadius = 1.0f;

		std::map<std::string, BoneInfo> m_BoneInfoMap;
		MEM::Ref<VertexArrayBuffer> _Vao;
		int m_BoneCounter = 0;

		void LoadSkeletalModel();
		void ProcessNode(aiNode* node, const aiScene* scene);
		Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
		void LoadTexture(aiMaterial* material, const MEM::Ref<Material>& mat, aiTextureType type, TextureType ourType, const aiScene* scene);
		MEM::Ref<Texture2D> LoadEmbeddedTexture(const aiTexture* texture);
		MEM::Ref<Material> LoadMaterial(aiMaterial* material, const aiScene* scene);

		void SetVertexBoneDataToDefault(Vertex& vertex);
		void SetVertexBoneData(Vertex& vertex, int boneID, float weight);
		void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);
	};
}