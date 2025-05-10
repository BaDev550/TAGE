#pragma once
#include <string>
#include "TAGE/Core/AssetManager/Asset.h"

namespace TAGE::RENDERER {
	enum class TextureType : uint8_t{
		Diffuse,
		Spec,
		Normal,
		Ambient,
		Metallic,
		Roughness
	};
	class Texture : ASSET::Asset 
	{
	public:
		virtual ~Texture() = default;
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual void Unbind() const = 0;
		
		virtual uint32_t GetID() const = 0;
		virtual int GetWidth() const = 0;
		virtual int GetHeight() const = 0;

		virtual ASSET::AssetType GetType() const override { return ASSET::AssetType::TEXTURE; }
	};

	class Texture2D : public Texture {
	public:
		Texture2D(const std::string& path, TextureType type = TextureType::Diffuse);
		Texture2D(uint32_t textureID, uint32_t width, uint32_t height);
		Texture2D(uint8_t* data, uint32_t size, bool isEmbeddedCompressed);
		Texture2D(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels);
		~Texture2D();

		void Bind(uint32_t slot = 0) const override;
		void Unbind() const override;
		void CreateCheckerboardTexture();

		uint32_t GetID() const override{ return _RendererID; }
		int GetWidth() const override { return _Width; }
		int GetHeight() const override { return _Height; }
	private:
		const char* _FilePath;
		uint32_t _RendererID = 0;
		int _Width = 0, _Height = 0, _Channels = 0;
		TextureType _Type;
	};

	class TextureCubeMap : public Texture {
	public:
		TextureCubeMap(const std::vector<std::string>& faces);
		~TextureCubeMap();

		void Bind(uint32_t slot = 0) const override;
		void Unbind() const override;

		uint32_t GetID() const override { return _RendererID; }
		int GetWidth() const override { return _Width; }
		int GetHeight() const override { return _Height; }
	private:
		uint32_t _RendererID = 0;
		int _Width = 0, _Height = 0, _Channels = 0;
	};
}