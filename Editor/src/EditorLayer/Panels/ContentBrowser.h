#pragma once

#include "Panel.h"
#include "TAGE/TAGE.h"
#include <filesystem>

namespace TAGE::EDITOR {
    class ContentBrowser : public Panel {
    public:
        ContentBrowser();

        void SetProjectDirectory(const std::filesystem::path& path);
        void OnImGuiRender() override;
        MEM::Ref<Texture2D> GetModelThumbnail(const std::string& modelPath);

    private:
        std::filesystem::path _BaseDirectory;
        std::filesystem::path _CurrentDirectory;
        std::filesystem::path _SelectedAsset;
		MEM::Ref<TEThubnailRenderer> _ThumbnailRenderer;
    };
}