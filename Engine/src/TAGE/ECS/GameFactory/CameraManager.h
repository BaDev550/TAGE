#pragma once

#include "TAGE/Core/Utilities/Memory.h"
#include "TAGE/Renderer/Camera.h"

namespace TAGE {
    class CameraManager
    {
    public:
        void SetEditorCamera(MEM::Ref<TAGE::RENDERER::EditorCamera> editorCam) { _EditorCamera = editorCam; }
        void SetGameCamera(MEM::Ref<TAGE::RENDERER::Camera> gameCam) { _GameCamera = gameCam; }

        MEM::Ref<TAGE::RENDERER::Camera> GetActiveCamera() const { return _ActiveCamera; }

        void UseEditorCamera()
        {
            _ActiveCamera = _EditorCamera;
        }

        void UseGameCamera()
        {
            _ActiveCamera = _GameCamera;
        }

        void ClearActiveCamera()
        {
            _ActiveCamera = nullptr;
        }

        bool HasActiveCamera() const { return _ActiveCamera != nullptr; }

    private:
        MEM::Ref<TAGE::RENDERER::EditorCamera> _EditorCamera = nullptr;
        MEM::Ref<TAGE::RENDERER::Camera> _GameCamera = nullptr;
        MEM::Ref<TAGE::RENDERER::Camera> _ActiveCamera = nullptr;
    };
}