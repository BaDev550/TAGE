#include "ContentBrowserPanel.h"
#include "imgui.h"

ContentBrowserPanel::ContentBrowserPanel() {
    _CurrentDirectory = std::filesystem::path("Assets");

    AddIcon("I_Folder", "Assets/Textures/Icons/Folder.png");
    AddIcon("I_File", "Assets/Textures/Icons/File.png");
}

void ContentBrowserPanel::Begin()
{
    ImGui::Begin("Asset Browser");

    if (ImGui::BeginPopupContextWindow("ContentBrowserContext", ImGuiPopupFlags_MouseButtonRight | ImGuiPopupFlags_NoOpenOverItems)) {
        if (ImGui::MenuItem("New Folder")) {
            _WantsToCreateFolder = true;
        }
        ImGui::Text("Classes");
        if (ImGui::MenuItem("Material")) {
        }
        if (ImGui::MenuItem("Scene")) {
            _WantsToCreateScene = true;
        }
        ImGui::EndPopup();
    }

    if (_CurrentDirectory != std::filesystem::path("Assets")) {
        if (ImGui::Button("<-")) {
            _CurrentDirectory = _CurrentDirectory.parent_path();
        }
    }

    ImGui::Separator();

    for (const auto& entry : std::filesystem::directory_iterator(_CurrentDirectory)) {
        const auto& path = entry.path();
        std::string name = path.filename().string();
        bool isDir = entry.is_directory();

        ImGui::PushID(name.c_str());

        uint32_t icon = isDir ? GetIcon("I_Folder") : GetIcon("I_File");
        if (icon != 0) {
            ImGui::Image((ImTextureID)(void*)icon, ImVec2(16, 16));
            ImGui::SameLine();
        }

        if (isDir) {
            bool opened = ImGui::TreeNodeEx(name.c_str(), ImGuiTreeNodeFlags_SpanFullWidth);
            if (ImGui::BeginDragDropSource()) {
                const std::string pathStr = path.string();
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", pathStr.c_str(), pathStr.size() + 1);
                ImGui::Text("Move: %s", name.c_str());
                ImGui::EndDragDropSource();
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
                _CurrentDirectory /= name;
            }

            if (opened) {
                ImGui::TreePop();
            }
        }
        else {
            ImGui::Text("%s", name.c_str());

            //if (ImGui::BeginDragDropSource()) {
            //    const std::string pathStr = path.string();
            //    ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", pathStr.c_str(), pathStr.size() + 1);
            //    ImGui::Text("Move: %s", name.c_str());
            //    ImGui::EndDragDropSource();
            //}

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left)) {
            }
        }

        ImGui::PopID();
    }

    ImGui::End();
}
