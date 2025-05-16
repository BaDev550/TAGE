#pragma once

#pragma once

#include "TAGE/Renderer/Buffers/Framebuffer.h"
#include "TAGE/Renderer/Shader/Shader.h"

namespace TAGE::RENDERER {
	struct PostProcessEffects {
		float Exposure;
		bool Bloom;
		bool MotionBlur;

		PostProcessEffects(float exposure = 0.01f, bool bloom = true, bool motionBlur = false) : 
			Exposure(exposure),
			Bloom(bloom),
			MotionBlur(motionBlur) {}
	};
	enum class RenderMode;
	class PostProcessPass
	{
	public:
		PostProcessPass(const MEM::Ref<Framebuffer>& inputFramebuffer, const MEM::Ref<Shader>& postShader);
		
		void Execute(RenderMode mode, PostProcessEffects effects = PostProcessEffects());

	private:
		MEM::Ref<Framebuffer> _InputFramebuffer;
		MEM::Ref<Shader> _PostShader;
	};
}