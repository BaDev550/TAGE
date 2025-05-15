#include "tagepch.h"
#include "Renderer.h"
#include <glad/glad.h>
#include "Shader/ShaderLibrary.h"

#include "TAGE/Core/Profiler/Profiler.h"

namespace TAGE::RENDERER {
	RenderSceneData Renderer::_SceneData;

	void Renderer::Init(Window* window)
	{
		_Window.reset(window);
		_Shader = ShaderLibrary::Load("core", "../Engine/shaders/Core_Vertex.glsl", "../Engine/shaders/Core_Fragment.glsl");
		_FBShader = ShaderLibrary::Load("framebuffer", "../Engine/shaders/Framebuffer_Vertex.glsl", "../Engine/shaders/Framebuffer_Fragment.glsl");
		_SkyBoxShader = ShaderLibrary::Load("skybox", "../Engine/shaders/Skybox_Vertex.glsl", "../Engine/shaders/Skybox_Fragment.glsl");
		_ShadowShader = ShaderLibrary::Load("shadow", "../Engine/shaders/Shadow_Vertex.glsl", "../Engine/shaders/Shadow_Fragment.glsl");
		_LineShader = ShaderLibrary::Load("line", "../Engine/shaders/Debug_Line_Vertex.glsl", "../Engine/shaders/Debug_Line_Fragment.glsl");
		_Framebuffer = MEM::CreateRef<Framebuffer>(_Window->GetWidth(), _Window->GetHeight(), _FBShader);
		_ShadowMap = MEM::CreateRef<ShadowMap>(2048, 2048);

		_SkyBox = MEM::CreateRef<Skybox>();

		_ScenePass = MEM::CreateRef<RenderPass>("Scene", _Framebuffer);
		_PostProcessPass = MEM::CreateRef<PostProcessPass>(_Framebuffer, _FBShader);

		_PostProcessEffects = PostProcessEffects(1.460f);
		DEBUG::DebugRenderer::Init();
	}

	void Renderer::BeginScene(const MEM::Ref<Camera>& cam)
	{
		cam->SetAspectRatio((float)_Window->GetWidth() / (float)_Window->GetHeight());
		_Framebuffer->Resize(_Window->GetWidth(), _Window->GetHeight());
		glm::mat4 vp = cam->GetProjectionMatrix() * cam->GetViewMatrix();
		_SceneData.viewProjectionMatrix = vp;

		_ScenePass->Begin();

		_Shader->Bind();
		_Shader->SetUniform("viewProj", vp);
		_Shader->SetUniform("u_Light.direction", _SceneLight.Pos);
		_Shader->SetUniform("u_Light.color", _SceneLight.Color);
		_Shader->SetUniform("u_Light.intensity", _SceneLight.Intensity);
		_Shader->SetUniform("u_CameraPosition", cam->GetPosition());

		_LineShader->Bind();
		_LineShader->SetUniform("u_ViewProj", vp);

		_SkyBoxShader->Bind();
		_SkyBoxShader->SetUniform("view", glm::mat4(glm::mat3(cam->GetViewMatrix())));
		_SkyBoxShader->SetUniform("projection", cam->GetProjectionMatrix());

		_SkyBox->DrawSkybox();
	}

	void Renderer::EndScene()
	{
		_ScenePass->End();
		_PostProcessPass->Execute(_PostProcessEffects);
	}

	void Renderer::BeginShadowMap()
	{
		Clear();
		TE_PROFILE_SCOPE("Shadow Pass");

		glm::mat4 lightProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, 75.0f);
		glm::mat4 lightView = glm::lookAt(
			_SceneLight.Pos,
			glm::vec3(0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f)
		);
		glm::mat4 lightSpaceMatrix = lightProjection * lightView;

		_ShadowShader->Bind();
		_ShadowMap->Bind();
		_ShadowShader->SetUniform("u_LightSpaceMatrix", lightSpaceMatrix);

		_Shader->Bind();
		_ShadowMap->BindDepthTexture(5);
		_Shader->SetUniform("shadowMap", 5);
		_Shader->SetUniform("u_LightSpaceMatrix", lightSpaceMatrix);
	}

	void Renderer::EndShadowMap()
	{
		_ShadowMap->Unbind();
	}

	void Renderer::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void Renderer::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Renderer::DrawIndexed(uint32_t count) {
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}
	void Renderer::DrawVertex(uint32_t size)
	{
		glDrawArrays(GL_TRIANGLES, 0, size);
	}
	void Renderer::Submit(const MEM::Ref<Shader>& shader, MEM::Ref<VertexArrayBuffer> vao, glm::mat4 transform)
	{
		shader->Bind();
		shader->SetUniform("u_Model", transform);
		vao->Bind();
		DrawIndexed(vao->GetIndexBuffer()->GetCount());
	}
	void Renderer::SetLight(const Light& light)
	{
		_SceneLight = light;
	}
}
