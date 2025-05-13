#include "tagepch.h"
#include "Model.h"
#include "TAGE/Renderer/Shader/ShaderLibrary.h"

namespace TAGE::RENDERER {
	Model::Model(const std::string& path, EMeshType type)
		: _MeshType(type)
	{
		_Vao = MEM::CreateRef<VertexArrayBuffer>();
		LoadModel(path);

		if (type == EMeshType::SKELETAL)
			LoadSkeletalModel();
	}

	void Model::Draw(glm::mat4 transform, const MEM::Ref<RENDERER::Shader>& shader) const
	{
		glm::mat4 centerOffset = glm::translate(glm::mat4(1.0f), -_CenterOffset);
		glm::mat4 finalTransform = transform * centerOffset;

		for (const auto& mesh : _Meshes) {
			mesh.MaterialR->Bind(shader);
			Renderer::Submit(shader, mesh.VAO, finalTransform);
		}
	}

	void Model::Reset()
	{
		_Meshes.clear();
		m_BoneInfoMap.clear();
	}

	void Model::LoadSkeletalModel()
	{
		_Skeletal = MEM::CreateScope<Skeletal>(this);
	}

	void Model::LoadModel(const std::string& path)
	{
		Assimp::Importer importer;
		const aiScene* _Scene = importer.ReadFile(path,
			aiProcess_Triangulate |
			aiProcess_FlipUVs |
			aiProcess_CalcTangentSpace |
			aiProcess_GenNormals |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_OptimizeMeshes |
			aiProcess_ImproveCacheLocality 
		);

		ENGINE_ASSERT(_Scene && _Scene->mRootNode, "Failed to load model: {}", path);
		_Directory = path.substr(0, path.find_last_of('/'));

		ProcessNode(_Scene->mRootNode, _Scene);
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0; i < node->mNumMeshes; ++i) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			_Meshes.push_back(ProcessMesh(mesh, scene));
		}

		for (uint32_t i = 0; i < node->mNumChildren; ++i) {
			ProcessNode(node->mChildren[i], scene);
		}
	}
	
	Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;
		glm::vec3 minBounds = glm::vec3(std::numeric_limits<float>::max());
		glm::vec3 maxBounds = glm::vec3(std::numeric_limits<float>::lowest());

		for (uint32_t i = 0; i < mesh->mNumVertices; ++i) {
			Vertex v;
			SetVertexBoneDataToDefault(v);
			v.Position[0] = mesh->mVertices[i].x;
			v.Position[1] = mesh->mVertices[i].y;
			v.Position[2] = mesh->mVertices[i].z;

			glm::vec3 pos = { v.Position[0], v.Position[1], v.Position[2] };
			minBounds = glm::min(minBounds, pos);
			maxBounds = glm::max(maxBounds, pos);

			if (mesh->HasNormals()) {
				v.Normal[0] = mesh->mNormals[i].x;
				v.Normal[1] = mesh->mNormals[i].y;
				v.Normal[2] = mesh->mNormals[i].z;
			}
			if (mesh->HasTextureCoords(0)) {
				v.TexCoords[0] = mesh->mTextureCoords[0][i].x;
				v.TexCoords[1] = mesh->mTextureCoords[0][i].y;
			}
			else {
				v.TexCoords[0] = 0.0f;
				v.TexCoords[1] = 0.0f;
			}
			if (mesh->HasTangentsAndBitangents()) {
				v.Tangent[0] = mesh->mTangents[i].x;
				v.Tangent[1] = mesh->mTangents[i].y;
				v.Tangent[2] = mesh->mTangents[i].z;

				v.Bitangent[0] = mesh->mBitangents[i].x;
				v.Bitangent[1] = mesh->mBitangents[i].y;
				v.Bitangent[2] = mesh->mBitangents[i].z;
			}

			vertices.push_back(v);
		}
		if(_MeshType == EMeshType::SKELETAL)
			ExtractBoneWeightForVertices(vertices, mesh, scene);

		for (uint32_t i = 0; i < mesh->mNumFaces; ++i) {
			for (uint32_t j = 0; j < mesh->mFaces[i].mNumIndices; ++j) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);
			}
		}

		MEM::Ref<VertexBuffer> vertexBuffer = MEM::CreateRef<VertexBuffer>(vertices.data(), vertices.size() * sizeof(Vertex));
		BufferLayout layout = {
			{ ShaderDataType::Vec3, "aPos" },
			{ ShaderDataType::Vec2, "aTexCoord" },
			{ ShaderDataType::Vec3, "aNormal" },
			{ ShaderDataType::Vec3, "aTangent" },
			{ ShaderDataType::Vec3, "aBitangent" },
			{ ShaderDataType::Vec4, "aBoneIDs" },
			{ ShaderDataType::Vec4, "aWeights" }
		};
		vertexBuffer->SetLayout(layout);

		_Vao->AddVertexBuffer(vertexBuffer);

		MEM::Ref<IndexBuffer> indexBuffer =  MEM::CreateRef<IndexBuffer>(indices.data(), indices.size());
		_Vao->SetIndexBuffer(indexBuffer);

		MEM::Ref<Material> material = LoadMaterial(scene->mMaterials[mesh->mMaterialIndex], scene);
		CORE_LOG_INFO("Mesh vertex count: {}", mesh->mNumVertices);

		glm::vec3 center = (minBounds + maxBounds) * 0.5f;
		for (auto& v : vertices) {
			glm::vec3 p(v.Position[0], v.Position[1], v.Position[2]);
			glm::vec3 centered = p - center;
			v.Position[0] = centered.x;
			v.Position[1] = centered.y;
			v.Position[2] = centered.z;
		}
		_CenterOffset = center;

		return { _Vao, material, (int)indices.size() };
	}

	void Model::LoadTexture(aiMaterial* material, const MEM::Ref<Material>& mat, aiTextureType type, TextureType ourType, const aiScene* scene) {
		if (material->GetTextureCount(type) > 0) {
			aiString str;
			material->GetTexture(type, 0, &str);
			std::string texPath = str.C_Str();

			if (!texPath.empty() && texPath[0] == '*') {
				int texIndex = std::atoi(texPath.c_str() + 1);
				if (scene && texIndex < scene->mNumTextures) {
					const aiTexture* texture = scene->mTextures[texIndex];
					if (texture && texture->pcData && texture->mWidth > 0) {
						const MEM::Ref<Texture2D> tex = LoadEmbeddedTexture(texture);
						if (tex) {
							mat->SetTexture(ourType, tex);
							return;
						}
					}
				}
			}
			else {
				std::string fullPath = _Directory + "/" + texPath;
				if (ASSET::AssetManager::Has<Texture2D>(fullPath)) {
					mat->SetTexture(ourType, ASSET::AssetManager::Get<Texture2D>(fullPath));
					return;
				}
				else if (!texPath.empty()) {
					auto texture = ASSET::AssetManager::Load<Texture2D>(fullPath);
					if (texture) {
						mat->SetTexture(ourType, texture);
						return;
					}
				}
			}
		}

		auto texture = ASSET::AssetManager::Load<Texture2D>("");
		mat->SetTexture(ourType, texture);
	}

	MEM::Ref<Material> Model::LoadMaterial(aiMaterial* material, const aiScene* scene)
	{
		const MEM::Ref<Material> Mmaterial = MEM::CreateRef<Material>();

		LoadTexture(material, Mmaterial, aiTextureType_DIFFUSE, TextureType::Diffuse, scene);
		LoadTexture(material, Mmaterial, aiTextureType_SPECULAR, TextureType::Spec, scene);
		LoadTexture(material, Mmaterial, aiTextureType_NORMALS, TextureType::Normal, scene);
		LoadTexture(material, Mmaterial, aiTextureType_AMBIENT, TextureType::Ambient, scene);
		LoadTexture(material, Mmaterial, aiTextureType_METALNESS, TextureType::Metallic, scene);
		LoadTexture(material, Mmaterial, aiTextureType_DIFFUSE_ROUGHNESS, TextureType::Ambient, scene);
		
		return Mmaterial;
	}
	void Model::SetVertexBoneDataToDefault(Vertex& vertex)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			vertex.BoneIDs[i] = -1;
			vertex.Weights[i] = 0.0f;
		}
	}
	void Model::SetVertexBoneData(Vertex& vertex, int boneID, float weight)
	{
		for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
		{
			if (vertex.BoneIDs[i] < 0)
			{
				vertex.BoneIDs[i] = boneID;
				vertex.Weights[i] = weight;
				break;
			}
		}
	}
	void Model::ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene)
	{
		auto& boneInfoMap = m_BoneInfoMap;
		int& boneCount = m_BoneCounter;
		
		for (int boneIndex = 0; boneIndex < mesh->mNumBones; ++boneIndex)
		{
			int boneID = -1;
			std::string boneName = mesh->mBones[boneIndex]->mName.C_Str();
			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				BoneInfo newBoneInfo;
				newBoneInfo.id = boneCount;
				newBoneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[boneIndex]->mOffsetMatrix);
				boneInfoMap[boneName] = newBoneInfo;
				boneID = boneCount;
				boneCount++;
			}
			else
			{
				boneID = boneInfoMap[boneName].id;
			}
			ENGINE_ASSERT(boneID != -1, "Invanlid bone ID");
			auto weights = mesh->mBones[boneIndex]->mWeights;
			int numWeights = mesh->mBones[boneIndex]->mNumWeights;

			for (int weightIndex = 0; weightIndex < numWeights; ++weightIndex)
			{
				int vertexId = weights[weightIndex].mVertexId;
				float weight = weights[weightIndex].mWeight;
				ENGINE_ASSERT(vertexId <= vertices.size(), "vertexId out of bound");
				SetVertexBoneData(vertices[vertexId], boneID, weight);
			}
		}
	}

	MEM::Ref<Texture2D> Model::LoadEmbeddedTexture(const aiTexture* texture) {
		if (texture->mHeight == 0) {
			return MEM::CreateRef<Texture2D>(
				(uint8_t*)texture->pcData, texture->mWidth, true
			);
		}
		else {
			uint32_t width = texture->mWidth;
			uint32_t height = texture->mHeight;
			uint8_t* data = reinterpret_cast<uint8_t*>(texture->pcData);
			const auto& tex = MEM::CreateRef<Texture2D>(data, width, height, 4);
			return tex;
		}
	}
}