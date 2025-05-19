#pragma once

#include "TAGE/Core/AssetManager/AssetImporter.h"
#include "TAGE/Renderer/Material/Texture.h"
#include "TAGE/Core/Utilities/Logger.h"

namespace TAGE::ASSET {
    class TextureImporter : public AssetImporter {
    public:
        MEM::Ref<Asset> Import(const std::filesystem::path& source, const std::filesystem::path& destination) override
        {
            YAML::Emitter outYaml;
            outYaml << YAML::BeginMap;
            outYaml << YAML::Key << "importSource" << YAML::Value << source.filename().string();
            outYaml << YAML::Key << "importSettings" << YAML::Value << YAML::BeginMap;
            outYaml << YAML::Key << "sRGB" << YAML::Value << true;
            outYaml << YAML::Key << "generateMipmaps" << YAML::Value << true;
            outYaml << YAML::EndMap;
            outYaml << YAML::EndMap;

            std::filesystem::path metaPath = destination;
            metaPath.replace_extension(".meta-data");

            std::ofstream metaOut(metaPath);
            if (!metaOut.is_open()) {
                CORE_LOG_ERROR("Failed to write meta-data file: {}", metaPath.string());
                return nullptr;
            }

            metaOut << outYaml.c_str();

            auto texture = MEM::CreateRef<RENDERER::Texture2D>(source.string());
            texture->path = destination.string();
            texture->handle = UUID();
            return texture;
        }

        std::vector<std::string> GetSupportedExtensions() const override {
            return { ".png", ".jpg", ".jpeg", ".tga", ".bmp", ".hdr", ".psd", ".gif" };
        }
    };
}
