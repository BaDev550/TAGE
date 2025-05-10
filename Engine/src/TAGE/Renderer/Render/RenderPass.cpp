#include "tagepch.h"
#include "RenderPass.h"
#include "TAGE/Renderer/Renderer.h"

namespace TAGE::RENDERER {
	RenderPass::RenderPass(const std::string& name, MEM::Ref<Framebuffer> framebuffer)
		: _Name(name), _Framebuffer(framebuffer) {}

	void RenderPass::Begin()
	{
		_Framebuffer->Bind();
		Renderer::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Renderer::Clear();
	}

	void RenderPass::End()
	{
		_Framebuffer->Unbind();
	}
}