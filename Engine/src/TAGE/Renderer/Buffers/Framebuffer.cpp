#include "tagepch.h"
#include "Framebuffer.h"
#include <GLAD/glad.h>
#include "TAGE/Renderer/Renderer.h"

namespace TAGE::RENDERER {
	Framebuffer::Framebuffer(uint32_t width, uint32_t height, const MEM::Ref<Shader>& shader) : _Width(width), _Height(height), FBShader(shader)
	{
		glCreateFramebuffers(1, &_RendererID);

		// Color Attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &_ColorAttachment);
		glTextureStorage2D(_ColorAttachment, 1, GL_RGBA8, width, height);
		glTextureParameteri(_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glNamedFramebufferTexture(_RendererID, GL_COLOR_ATTACHMENT0, _ColorAttachment, 0);

		// Bloom Attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &_BloomAttachment);
		glTextureStorage2D(_BloomAttachment, 1, GL_RGBA16F, width, height);
		glTextureParameteri(_BloomAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_BloomAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glNamedFramebufferTexture(_RendererID, GL_COLOR_ATTACHMENT1, _BloomAttachment, 0);

		GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glNamedFramebufferDrawBuffers(_RendererID, 2, buffers);

		// Depth Attachment
		glCreateTextures(GL_TEXTURE_2D, 1, &_DepthAttachment);
		glTextureStorage2D(_DepthAttachment, 1, GL_DEPTH24_STENCIL8, width, height);
		glTextureParameteri(_DepthAttachment, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(_DepthAttachment, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glNamedFramebufferTexture(_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, _DepthAttachment, 0);

		GLenum status = glCheckNamedFramebufferStatus(_RendererID, GL_FRAMEBUFFER);
		ENGINE_ASSERT(status == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is not complete!");

		CreateQuad();
	}

	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &_RendererID);
		glDeleteTextures(1, &_ColorAttachment);
		glDeleteTextures(1, &_DepthAttachment);
	}

	void Framebuffer::Bind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _RendererID);
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		glViewport(0, 0, _Width, _Height);
	}

	void Framebuffer::Unbind() const
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glDrawBuffer(GL_BACK);
		glReadBuffer(GL_BACK);
	}

	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > 8192 || height > 8192) return;

		_Width = width;
		_Height = height;

		glDeleteTextures(1, &_ColorAttachment);
		glDeleteTextures(1, &_DepthAttachment);
		glDeleteTextures(1, &_BloomAttachment);

		glCreateTextures(GL_TEXTURE_2D, 1, &_ColorAttachment);
		glTextureStorage2D(_ColorAttachment, 1, GL_RGBA8, width, height);
		glTextureParameteri(_ColorAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_ColorAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glNamedFramebufferTexture(_RendererID, GL_COLOR_ATTACHMENT0, _ColorAttachment, 0);

		glCreateTextures(GL_TEXTURE_2D, 1, &_BloomAttachment);
		glTextureStorage2D(_BloomAttachment, 1, GL_RGBA16F, width, height);
		glTextureParameteri(_BloomAttachment, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(_BloomAttachment, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glNamedFramebufferTexture(_RendererID, GL_COLOR_ATTACHMENT1, _BloomAttachment, 0);

		GLenum buffers[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
		glNamedFramebufferDrawBuffers(_RendererID, 2, buffers);

		glCreateTextures(GL_TEXTURE_2D, 1, &_DepthAttachment);
		glTextureStorage2D(_DepthAttachment, 1, GL_DEPTH24_STENCIL8, width, height);
		glTextureParameteri(_DepthAttachment, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(_DepthAttachment, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glNamedFramebufferTexture(_RendererID, GL_DEPTH_STENCIL_ATTACHMENT, _DepthAttachment, 0);
	}
	void Framebuffer::DrawFullQuad()
	{
		FBShader->Bind();
		FBVAO->Bind();
		FBTexture->Bind(0);
		FBBloomTexture->Bind(1);
		FBShader->SetUniform("screenTexture", 0);
		FBShader->SetUniform("bloomBlur", 1);
		Renderer::DrawIndexed(FBVAO->GetIndexBuffer()->GetCount());
	}
	struct FBVertex {
		glm::vec3 Position;
		glm::vec2 TexCoords;
	};

	void Framebuffer::CreateQuad()
	{
		FBShader->Bind();

		std::vector<FBVertex> vertices = {
			{{-1.0f, -1.0f, 0.0f}, {0.0f, 0.0f}},
			{{ 1.0f, -1.0f, 0.0f}, {1.0f, 0.0f}},
			{{ 1.0f,  1.0f, 0.0f}, {1.0f, 1.0f}},
			{{-1.0f,  1.0f, 0.0f}, {0.0f, 1.0f}}
		};

		std::vector<uint32_t> indices = {
			0, 1, 2,
			2, 3, 0
		};

		MEM::Ref<VertexBuffer> vertexBuffer = MEM::CreateRef<VertexBuffer>(vertices.data(), vertices.size() * sizeof(FBVertex));
		MEM::Ref<IndexBuffer> indexBuffer = MEM::CreateRef<IndexBuffer>(indices.data(), indices.size());

		BufferLayout layout = {
			{ ShaderDataType::Vec3, "aPos" },
			{ ShaderDataType::Vec2, "aTexCoord" }
		};
		vertexBuffer->SetLayout(layout);

		FBVAO = MEM::CreateRef<VertexArrayBuffer>();
		FBVAO->AddVertexBuffer(vertexBuffer);
		FBVAO->SetIndexBuffer(indexBuffer);

		FBTexture = MEM::CreateRef<Texture2D>(_ColorAttachment, _Width, _Height);
		FBBloomTexture = MEM::CreateRef<Texture2D>(_BloomAttachment, _Width, _Height);
	}
}