#pragma once

#include <glm/glm.hpp>
#include "Shader/Shader.h"
#include "Buffers/Framebuffer.h"
#include "Skybox/Skybox.h"
#include "Camera.h"
#include "TAGE/Window/Window.h"
#include "TAGE/Renderer/Render/RenderPass.h"
#include "TAGE/Renderer/Render/PostProcessPass.h"
#include "TAGE/Renderer/Render/Light.h"
#include "TAGE/Renderer/Shadow/ShadowMap.h"

namespace TAGE::RENDERER {
	class Renderer {
	public:
		Renderer() {}
		~Renderer() {
			_Window.release();
		}

		void Init(Window* window);
		void BeginScene(const MEM::Ref<Camera>& cam);
		void EndScene();

		void BeginShadowMap();
		void EndShadowMap();

		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void DrawIndexed(uint32_t count);
		static void DrawVertex(uint32_t size);
		static void Submit(const MEM::Ref<Shader>& shader, MEM::Ref<VertexArrayBuffer> vao, glm::mat4 transform);
		void SetLight(const Light& light);

		Framebuffer* GetFramebuffer() { return _Framebuffer.get(); }
		Light _SceneLight;
	private:
		MEM::Ref<Shader> _Shader;
		MEM::Ref<Shader> _ShadowShader;
		MEM::Ref<Shader> _FBShader;
		MEM::Ref<Shader> _SkyBoxShader;
		MEM::Ref<Framebuffer> _Framebuffer;
		MEM::Ref<ShadowMap> _ShadowMap;
		MEM::Ref<Skybox> _SkyBox;
	private:
		MEM::Scope<Window> _Window;
		MEM::Ref<RenderPass> _ScenePass;
		MEM::Ref<PostProcessPass> _PostProcessPass;
		PostProcessEffects _PostProcessEffects;
	};
}
