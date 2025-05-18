#pragma once

#include "TAGE/Renderer/Model/Model.h"
#include "TAGE/Renderer/Camera.h"
#include "TAGE/Renderer/Buffers/Framebuffer.h"
#include "TAGE/ECS/World/Scene.h"
#include "TAGE/Renderer/Material/Texture.h"
#include "TAGE/Renderer/Shader/ShaderLibrary.h"
#include "TAGE/Renderer/Renderer.h"
#include "TAGE/ECS/Components/RenderingComponents.h"

namespace TAGE::RENDERER {
    class ThumbnailRenderer
    {
    public:
        ThumbnailRenderer(uint32_t width, uint32_t height);
        MEM::Ref<Texture2D> RenderModel(const MEM::Ref<Model>& model);

    private:
        MEM::Ref<Framebuffer> m_Framebuffer;
        MEM::Ref<Scene> m_RenderScene;
        MEM::Ref<EditorCamera> m_ThumbnailCamera;
        MEM::Ref<DirectionalLight> m_Light;
        uint32_t m_Width, m_Height;
    };
}