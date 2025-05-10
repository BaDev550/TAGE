#pragma once

#include <unordered_map>
#include <glm/glm.hpp>
#include "Texture.h"
#include "TAGE/Core/AssetManager/Asset.h"
#include "TAGE/Renderer/Shader/Shader.h"

namespace TAGE::RENDERER {
	class Material : public ASSET::Asset
	{
	public:
		Material() {}
		~Material() = default;

		void SetTexture(TextureType type, MEM::Ref<Texture> texture);
		const MEM::Ref<Texture>& GetTexture(TextureType type) const;

		void Bind(const MEM::Ref<RENDERER::Shader>& shader) const;
		void Unbind() const;

		virtual ASSET::AssetType GetType() const override { return ASSET::AssetType::MATERIAL; }

	private:
		std::unordered_map<TextureType, MEM::Ref<Texture>> _Textures;
	};
}