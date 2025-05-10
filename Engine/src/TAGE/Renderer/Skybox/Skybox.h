#pragma once

#include "TAGE/Renderer/Material/Texture.h"
#include "TAGE/Renderer/Shader/Shader.h"
#include "TAGE/Renderer/Buffers/Buffers.h"
#include "TAGE/Renderer/Buffers/VertexArrayBuffer.h"

namespace TAGE::RENDERER {
	class Skybox
	{
        std::vector<std::string> faces
        {
            "../Engine/resources/textures/skybox/right.jpg",
            "../Engine/resources/textures/skybox/left.jpg",
            "../Engine/resources/textures/skybox/top.jpg",
            "../Engine/resources/textures/skybox/bottom.jpg",
            "../Engine/resources/textures/skybox/front.jpg",
            "../Engine/resources/textures/skybox/back.jpg"
        };
	public:
        Skybox();

        void DrawSkybox();
    private:
        MEM::Ref<TextureCubeMap> _CubeMap;
        MEM::Ref<Shader> _SkyBoxShader;
        MEM::Ref<VertexArrayBuffer> _SKYBVertexArray;
        MEM::Ref<VertexBuffer> _VertexBuffer;
	};
}