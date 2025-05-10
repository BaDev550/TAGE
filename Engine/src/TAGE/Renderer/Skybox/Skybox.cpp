#include "tagepch.h"
#include "Skybox.h"
#include "glm/glm.hpp"
#include "TAGE/Renderer/Shader/ShaderLibrary.h"
#include "TAGE/Renderer/Renderer.h"
#include "GLAD/glad.h"

namespace TAGE::RENDERER {
    Skybox::Skybox()
    {
        _CubeMap = MEM::CreateRef<TextureCubeMap>(faces);
        _SkyBoxShader = ShaderLibrary::Get("skybox");

        std::vector<glm::vec3> skyboxVertices = {
            { -1.0f,  1.0f, -1.0f },
            { -1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f,  1.0f, -1.0f },
            { -1.0f,  1.0f, -1.0f },

            { -1.0f, -1.0f,  1.0f },
            { -1.0f, -1.0f, -1.0f },
            { -1.0f,  1.0f, -1.0f },
            { -1.0f,  1.0f, -1.0f },
            { -1.0f,  1.0f,  1.0f },
            { -1.0f, -1.0f,  1.0f },

            {  1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            {  1.0f,  1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },

            { -1.0f, -1.0f,  1.0f },
            { -1.0f,  1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            {  1.0f, -1.0f,  1.0f },
            { -1.0f, -1.0f,  1.0f },

            { -1.0f,  1.0f, -1.0f },
            {  1.0f,  1.0f, -1.0f },
            {  1.0f,  1.0f,  1.0f },
            {  1.0f,  1.0f,  1.0f },
            { -1.0f,  1.0f,  1.0f },
            { -1.0f,  1.0f, -1.0f },

            { -1.0f, -1.0f, -1.0f },
            { -1.0f, -1.0f,  1.0f },
            {  1.0f, -1.0f, -1.0f },
            {  1.0f, -1.0f, -1.0f },
            { -1.0f, -1.0f,  1.0f },
            {  1.0f, -1.0f,  1.0f }
        };

        _VertexBuffer = MEM::CreateRef<VertexBuffer>(
            skyboxVertices.data(), skyboxVertices.size() * sizeof(glm::vec3)
        );

        BufferLayout layout = {
            { ShaderDataType::Vec3, "aPos" }
        };
        _VertexBuffer->SetLayout(layout);

        _SKYBVertexArray = MEM::CreateRef<VertexArrayBuffer>();
        _SKYBVertexArray->AddVertexBuffer(_VertexBuffer);
    }
    void Skybox::DrawSkybox()
    {
        glDepthFunc(GL_LEQUAL);
        _SkyBoxShader->Bind();
        _CubeMap->Bind();
        _SKYBVertexArray->Bind();
        Renderer::DrawVertex(36);
        glDepthFunc(GL_LESS);
    }
}