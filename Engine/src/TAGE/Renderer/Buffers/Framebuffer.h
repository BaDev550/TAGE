#pragma once

#include "Buffers.h"
#include "VertexArrayBuffer.h"
#include "TAGE/Renderer/Material/Texture.h"
#include "TAGE/Renderer/Shader/Shader.h"

namespace TAGE::RENDERER {
	class Framebuffer
	{
	public:
		Framebuffer(uint32_t width, uint32_t height, const MEM::Ref<Shader>& shader);
		~Framebuffer();

		void Bind() const;
		void Unbind() const;
		void Resize(uint32_t width, uint32_t height);

		uint32_t GetColorAttachment() const { return _ColorAttachment; }
		uint32_t GetDepthAttachment() const { return _DepthAttachment; }

		uint32_t GetWidth() const { return _Width; }
		uint32_t GetHeight() const { return _Height; }

		void DrawFullQuad();
	private:
		uint32_t _RendererID = 0;
		uint32_t _ColorAttachment = 0;
		uint32_t _DepthAttachment = 0;
		uint32_t _BloomAttachment = 0;
		uint32_t _Width = 0, _Height = 0;
		MEM::Ref<VertexArrayBuffer> FBVAO;
		MEM::Ref<Shader> FBShader;
		MEM::Ref<Texture> FBTexture;
		MEM::Ref<Texture> FBBloomTexture;
	private:
		void CreateQuad();
	};
}