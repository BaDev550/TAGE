#pragma once

#include "TAGE/Core/AssetManager/AssetImporter.h"

#include "TAGE/Renderer/Buffers/Buffers.h"
#include "TAGE/Renderer/Model/Model.h"
#include "TAGE/Core/Utilities/Logger.h"

namespace TAGE::ASSET {
    class ModelImporter : public AssetImporter {
    public:
        MEM::Ref<Asset> Import(const std::filesystem::path& source, const std::filesystem::path& destination) override
		{
            YAML::Emitter outYaml;
            outYaml << YAML::BeginMap;
            outYaml << YAML::Key << "importSource" << YAML::Value << source.filename().string();
            outYaml << YAML::Key << "importSettings" << YAML::Value << YAML::BeginMap;
            outYaml << YAML::Key << "scale" << YAML::Value << 1.0f;
            outYaml << YAML::Key << "optimizeMesh" << YAML::Value << true;
            outYaml << YAML::Key << "generateNormals" << YAML::Value << false;
            outYaml << YAML::Key << "generateTangents" << YAML::Value << true;
            outYaml << YAML::EndMap;
            outYaml << YAML::EndMap;

            std::filesystem::path metaPath = destination;
            metaPath.replace_extension(".meta-data");

            std::ofstream metaOut(metaPath);
            if (!metaOut.is_open()) {
                CORE_LOG_ERROR("Failed to write meta-data file: {}", metaPath.string());
                return false;
            }

            metaOut << outYaml.c_str();
            
            auto model = MEM::CreateRef<RENDERER::Model>(source.string());
			model->path = destination.string();
			model->handle = UUID();
            return model;
		}

        std::vector<std::string> GetSupportedExtensions() const override {
            return { ".obj", ".fbx", ".dae", ".glb", ".gltf" };
        }
	};
}