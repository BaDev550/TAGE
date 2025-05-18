#pragma once

#include "TAGE/TAGE.h"
#include "Panels/ScenePanel.h"
#include "Panels/InspectorPanel.h"
#include "Panels/ViewportPanel.h"
#include "Panels/ToolbarPanel.h"
#include "Panels/OutputPanel.h"
#include "Panels/ContentBrowser.h"
#include <memory>

namespace TAGE::EDITOR {
    class EditorLayer : public TELayer {
    public:
        EditorLayer();
        virtual ~EditorLayer() = default;

        void SetDockspace();
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

    private:
        MEM::Ref<Scene> _EditorScene;
        MEM::Ref<Scene> _RuntimeScene;
		MEM::Ref<TEEditorCamera> _EditorCamera;

        ScenePanel _SceneHierarchyPanel;
		InspectorPanel _InspectorPanel;
		ViewportPanel _ViewportPanel;
		ToolBarPanel _ToolBarPanel;
		OutputPanel _OutputPanel;
		ContentBrowser _ContentBrowser;

        bool _ViewportFocused = false;
        bool _ViewportHovered = false;

        glm::vec2 _ViewportSize = { 0.0f, 0.0f };

        enum class SceneState { EDIT = 0, PLAY, SIMULATE };
        SceneState _SceneState = SceneState::EDIT;

    private:
        void OnScenePlay();
        void OnSceneStop();
    };

}