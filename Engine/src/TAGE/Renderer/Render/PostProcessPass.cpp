#include "tagepch.h"
#include "PostProcessPass.h"
#include "TAGE/Renderer/Renderer.h"

namespace TAGE::RENDERER {

	PostProcessPass::PostProcessPass(const MEM::Ref<Framebuffer>& inputFramebuffer, const MEM::Ref<Shader>& postShader)
		: _InputFramebuffer(inputFramebuffer), _PostShader(postShader) {}

	void PostProcessPass::Execute(RenderMode mode, PostProcessEffects effects)
	{
		Renderer::Clear();
		_PostShader->Bind(); 
		_PostShader->SetUniform("u_Exposure", effects.Exposure);
		_PostShader->SetUniform("u_BloomEnabled", effects.Bloom);

		if (mode == RenderMode::FULL)
			_InputFramebuffer->DrawFullQuad();
	}
}