#include "tagepch.h"
#include "System.h"
#include "TAGE/Core/Application/Application.h"

namespace TAGE::ECS {

    RenderSystem::RenderSystem(RENDERER::Renderer* renderer) {
        _Renderer = renderer;
        _Shader = ShaderLibrary::Get("core");
        _ShadowShader = ShaderLibrary::Get("shadow");
    }

    void RenderSystem::PassLightsToShader(entt::registry& registry) {

    }

    void RenderSystem::UpdateCamera(entt::registry& registry) {
        auto view = registry.view<TransformComponent, CameraComponent>();
        PassLightsToShader(registry);
        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            auto& camera = view.get<CameraComponent>(entity);
            transform.Position = camera.Camera->GetPosition();
            transform.Rotation = camera.Camera->GetRotation();
        }
    }

    void RenderSystem::RenderScene(entt::registry& registry, float dt, const MEM::Ref<RENDERER::Shader>& shader) {
        auto staticmeshview = registry.view<TransformComponent, StaticMeshComponent>();
        auto skeletalmeshview = registry.view<TransformComponent, SkeletalMeshComponent>();

        for (const auto& entity : staticmeshview) {
            auto& transform = registry.get<TransformComponent>(entity);
            auto& static_model = registry.get<StaticMeshComponent>(entity);
            static_model.Draw(shader, transform.GetMatrix());
        }

        for (const auto& entity : skeletalmeshview) {
            auto& transform = registry.get<TransformComponent>(entity);
            auto& skeletal_model = registry.get<SkeletalMeshComponent>(entity);

            if (registry.any_of<AnimatorComponent>(entity)) {
                auto& animator = registry.get<AnimatorComponent>(entity).AnimatorInstance;
                animator->UpdateAnimation(dt);

                auto transforms = animator->GetFinalBoneMatrices();
                for (int i = 0; i < transforms.size(); ++i)
                    _Shader->SetUniform("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);
            }

            skeletal_model.Draw(shader, transform.GetMatrix());
        }
    }

    void RenderSystem::Update(entt::registry& registry, float dt, SystemUpdateMode mode)
    {
        PassLightsToShader(registry);
        auto cameraView = registry.view<CameraComponent>();

        if (!cameraView.empty()) {
            auto entity = *cameraView.begin();
            auto activeCam = registry.get<CameraComponent>(entity);

            activeCam.Camera->OnUpdate(dt);

            _Renderer->BeginShadowMap();
            RenderScene(registry, dt, _ShadowShader);
            _Renderer->EndShadowMap();

            _Renderer->BeginScene(activeCam.Camera);
            Application::Get().UpdateLayers(dt);
            RenderScene(registry, dt, _Shader);
            _Renderer->EndScene();

            UpdateCamera(registry);
        }
    }
}