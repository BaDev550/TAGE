#pragma once
#include <filesystem>
#include <TAGE/TAGE.h>
#include "Panel.h"

class ContentBrowserPanel : public Panel {
public:
    ContentBrowserPanel();

    virtual void Begin() override;
private:
    std::filesystem::path _CurrentDirectory;

    std::string _NewFolderName = "New Folder";
    bool _WantsToCreateFolder = false;

    std::string _NewSceneName = "New Scene";
    bool _WantsToCreateScene = false;
};
