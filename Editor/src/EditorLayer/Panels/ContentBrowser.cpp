#include "ContentBrowser.h"

namespace TAGE::EDITOR {
    ContentBrowser::ContentBrowser()
    {
        _ThumbnailRenderer = MEM::CreateRef<ThumbnailRenderer>(128, 128);
    }
    void ContentBrowser::SetProjectDirectory(const std::filesystem::path& path)
	{
        _BaseDirectory = path;
        _CurrentDirectory = _BaseDirectory;
		AddIcon("I_File",   "Assets/Textures/Icons/File.png");
		AddIcon("I_Folder", "Assets/Textures/Icons/Folder.png");
	}
	void ContentBrowser::OnImGuiRender()
	{
        ImGui::Begin("Content Browser");

        if (_CurrentDirectory != _BaseDirectory)
        {
            if (ImGui::Button("<-"))
                _CurrentDirectory = _CurrentDirectory.parent_path();
        }

        float padding = 16.0f;
        float thumbnailSize = 64.0f;
        float cellSize = thumbnailSize + padding;

        float panelWidth = ImGui::GetContentRegionAvail().x;
        int columnCount = (int)(panelWidth / cellSize);
        if (columnCount < 1) columnCount = 1;

        ImGui::Columns(columnCount, 0, false);

        for (auto& entry : std::filesystem::directory_iterator(_CurrentDirectory))
        {
            const auto& path = entry.path();
            auto relativePath = std::filesystem::relative(path, _BaseDirectory);
            std::string filename = path.filename().string();
            auto ext = path.extension().string();

            uint32_t icon = entry.is_directory() ? GetIcon("I_Folder") : GetIcon("I_File");
            if (ext == ".png" || ext == ".jpg") {
                AddIcon(filename, path.string());
                icon = GetIcon(filename);
            }
            //else if (ext == ".fbx" || ext == ".obj" || ext == ".gltf" || ext == ".glb") {
            //    if (!HasIcon(filename)) {
            //        auto thumbnail = GetModelThumbnail(path.string());
            //        AddIcon(filename, thumbnail);
            //    }
            //    icon = GetIconTexture(filename)->GetID();
            //}

            ImGui::PushID(filename.c_str());
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

            ImGui::ImageButton("Asset", (ImTextureID)(uintptr_t)icon, {thumbnailSize, thumbnailSize});

            if (ImGui::BeginDragDropSource())
            {
                std::wstring wpath = relativePath.wstring();
                ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", wpath.c_str(), (wpath.size() + 1) * sizeof(wchar_t));
                ImGui::Text("%s", filename.c_str());
                ImGui::EndDragDropSource();
            }

            if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
            {
                if (entry.is_directory())
                    _CurrentDirectory /= path.filename();
            }

            if (ImGui::IsItemClicked())
            {
                _SelectedAsset = path;
            }

            ImGui::PopStyleColor();
            ImGui::TextWrapped("%s", filename.c_str());

            ImGui::NextColumn();
            ImGui::PopID();
        }

        ImGui::Columns(1);
        ImGui::End();
	}
    MEM::Ref<Texture2D> ContentBrowser::GetModelThumbnail(const std::string& modelPath)
    {
        auto model = MEM::CreateRef<Model>(modelPath);
        auto thumbnail = _ThumbnailRenderer->RenderModel(model);
        return thumbnail;
    }
}