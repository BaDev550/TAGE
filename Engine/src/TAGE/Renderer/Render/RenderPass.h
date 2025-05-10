#pragma once

#include "TAGE/Renderer/Buffers/Framebuffer.h"

namespace TAGE::RENDERER {
	class RenderPass
	{
	public:
		RenderPass(const std::string& name, MEM::Ref<Framebuffer> framebuffer);
		void Begin();
		void End();

		const std::string& GetName() const {}

	private:
		std::string _Name;
		MEM::Ref<Framebuffer> _Framebuffer;
	};
}