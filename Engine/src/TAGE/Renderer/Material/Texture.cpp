#include "tagepch.h"
#include "Texture.h"
#include <glad/glad.h>
#include "stb/stb_image.h"

namespace TAGE::RENDERER {
	void Texture2D::CreateCheckerboardTexture()
	{
		const int checkerSize = 8;
		const int texSize = 128;
		_Width = _Height = texSize;
		_Channels = 3;

		std::vector<unsigned char> data(_Width * _Height * _Channels);

		for (int y = 0; y < _Height; ++y) {
			for (int x = 0; x < _Width; ++x) {
				int checkerX = x / checkerSize;
				int checkerY = y / checkerSize;
				bool isWhite = (checkerX + checkerY) % 2 == 0;

				int index = (y * _Width + x) * _Channels;
				if (isWhite) {
					data[index + 0] = 200;
					data[index + 1] = 200;
					data[index + 2] = 200;
				}
				else {
					data[index + 0] = 50;
					data[index + 1] = 50;
					data[index + 2] = 50;
				}
			}
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &_RendererID);
		glTextureStorage2D(_RendererID, 1, GL_RGB8, _Width, _Height);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTextureSubImage2D(_RendererID, 0, 0, 0, _Width, _Height, GL_RGB, GL_UNSIGNED_BYTE, data.data());
	}

	Texture2D::Texture2D(const std::string& path, TextureType type) : _FilePath(path.c_str()), _Type(type)
	{
		unsigned char* data = stbi_load(path.c_str(), &_Width, &_Height, &_Channels, 0);
		if (!data || path.empty()) {
			CORE_LOG_WARN("Failed to load image at path: {0} Using fallback texture.", path);
			CreateCheckerboardTexture();
			return;
		}

		GLenum format = (_Channels == 4) ? GL_RGBA : (_Channels == 3) ? GL_RGB : GL_RED;
		glCreateTextures(GL_TEXTURE_2D, 1, &_RendererID);
		glTextureStorage2D(_RendererID, 1, GL_RGBA8, _Width, _Height);

		glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(_RendererID, 0, 0, 0, _Width, _Height, format, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}
	Texture2D::Texture2D(uint32_t textureID, uint32_t width, uint32_t height)
		: _RendererID(textureID), _Width(width), _Height(height)
	{
	}

	Texture2D::Texture2D(uint8_t* data, uint32_t size, bool isEmbeddedCompressed)
	{
		_Channels = 0;
		unsigned char* imageData = stbi_load_from_memory(data, size, &_Width, &_Height, &_Channels, 0);
		if (!imageData) {
			CreateCheckerboardTexture();
			return;
		}

		GLenum format = (_Channels == 4) ? GL_RGBA : (_Channels == 3) ? GL_RGB : GL_RED;
		glCreateTextures(GL_TEXTURE_2D, 1, &_RendererID);
		glTextureStorage2D(_RendererID, 1, GL_RGBA8, _Width, _Height);

		glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(_RendererID, 0, 0, 0, _Width, _Height, format, GL_UNSIGNED_BYTE, imageData);

		stbi_image_free(imageData);
	}

	Texture2D::Texture2D(uint8_t* data, uint32_t width, uint32_t height, uint32_t channels)
		: _Width(width), _Height(height), _Channels(channels)
	{
		GLenum format = (_Channels == 4) ? GL_RGBA : (_Channels == 3) ? GL_RGB : GL_RED;

		glCreateTextures(GL_TEXTURE_2D, 1, &_RendererID);
		glTextureStorage2D(_RendererID, 1, GL_RGBA8, _Width, _Height);

		glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureSubImage2D(_RendererID, 0, 0, 0, _Width, _Height, format, GL_UNSIGNED_BYTE, data);
	}

	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &_RendererID);
	}
	void Texture2D::Bind(uint32_t slot) const
	{
		if (_RendererID == 0)
			CORE_LOG_ERROR("INVANLID TEXTURE");
		glBindTextureUnit(slot, _RendererID);
	}
	void Texture2D::Unbind() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	TextureCubeMap::TextureCubeMap(const std::vector<std::string>& faces)
	{
		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &_RendererID);

		ENGINE_ASSERT(!faces.empty(), "Cubemap face paths are empty!");
		for (size_t i = 0; i < faces.size(); i++) {
			unsigned char* data = stbi_load(faces[i].c_str(), &_Width, &_Height, &_Channels, 0);
			ENGINE_ASSERT(data, "Failed to load texture image!");

			if (data) {
				glTextureStorage2D(_RendererID, 1, GL_RGB8, _Width, _Height);
				glTextureSubImage3D(
					_RendererID,
					0,                        
					0, 0, i,                  
					_Width, _Height, 1,
					GL_RGB, GL_UNSIGNED_BYTE,
					data
				);
			}
			else {
				stbi_image_free(data);
			}
		}
		glTextureParameteri(_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	}
	TextureCubeMap::~TextureCubeMap()
	{
		glDeleteTextures(1, &_RendererID);
	}
	void TextureCubeMap::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, _RendererID);
	}
	void TextureCubeMap::Unbind() const
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}
}