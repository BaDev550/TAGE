#include "tagepch.h"
#include "Material.h"

namespace TAGE::RENDERER {
    void Material::SetTexture(TextureType type, MEM::Ref<Texture> texture)
    {
        _Textures[type] = texture;
    }
    
    const MEM::Ref<Texture>& Material::GetTexture(TextureType type) const
    {
        static MEM::Ref<Texture> tempTexture = nullptr;
        auto it = _Textures.find(type);
        if (it != _Textures.end())
            return it->second;
        return tempTexture;
    }

    void Material::Bind(const MEM::Ref<RENDERER::Shader>& shader) const
    {
        shader->Bind();

        int slot = 0;
        for (const auto& [type, texture] : _Textures) {
            if (texture) {
                texture->Bind(slot);

                switch (type)
                {
                case TextureType::Diffuse: shader->SetUniform("u_Material.u_DiffuseMap",  slot); break;
                case TextureType::Spec:    shader->SetUniform("u_Material.u_SpecularMap", slot); break;
                case TextureType::Normal:  shader->SetUniform("u_Material.u_NormalMap",   slot); break;
                case TextureType::Ambient: shader->SetUniform("u_Material.u_AmbientTexture",  slot); break;
                case TextureType::Metallic:   shader->SetUniform("u_Material.u_MetallicMap", slot); break;
                case TextureType::Roughness:  shader->SetUniform("u_Material.u_RoughnessMap", slot); break;
                }
                slot++;
            }
        }
    }

    void Material::Unbind() const
    {
        for (const auto& [type, texture] : _Textures) {
            if (texture)
                texture->Unbind();
        }
    }
}