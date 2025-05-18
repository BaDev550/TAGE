#include "tagepch.h"
#include "ThumbnailRenderer.h"
#include "TAGE/Core/Application/Application.h"

namespace TAGE::RENDERER {
    ThumbnailRenderer::ThumbnailRenderer(uint32_t width, uint32_t height)
        : m_Width(width), m_Height(height)
    {
		auto _FBShader = ShaderLibrary::Get("framebuffer");
        m_Framebuffer = MEM::CreateRef<Framebuffer>(width, height, _FBShader);

        m_RenderScene = std::make_shared<Scene>();
        m_RenderScene->Init();

        m_ThumbnailCamera = std::make_shared<EditorCamera>(45.0f, (float)width / height, 0.1f, 100.0f);
        m_ThumbnailCamera->SetPosition({ 0.0f, 0.0f, 3.0f });

        m_Light = std::make_shared<DirectionalLight>();
        m_Light->Direction = { -1.0f, -1.0f, -1.0f };
        m_Light->Color = glm::vec3(1.0f);
    }

	MEM::Ref<Texture2D> ThumbnailRenderer::RenderModel(const MEM::Ref<Model>& model)
	{
        Application::Get().GetRenderer().BeginScene(m_ThumbnailCamera);
        m_Framebuffer->Bind();
        

        auto entity = m_RenderScene->CreateEntity("PreviewModel");
        auto& meshComp = entity->AddComponent<MeshComponent>();
        meshComp.LoadModel(model->GetPath());



        m_Framebuffer->Unbind();

        uint32_t textureID = m_Framebuffer->GetColorAttachment();
        return MEM::CreateRef<Texture2D>(textureID, m_Width, m_Height);
	}
}