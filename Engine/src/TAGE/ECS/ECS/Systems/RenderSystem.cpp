#include "tagepch.h"
#include "System.h"
#include "TAGE/Renderer/Shader/ShaderLibrary.h"
#include "TAGE/Core/Application/Application.h"

namespace TAGE::ECS{
#ifndef PHYSICS_DEBUGER_INCLUDED
    PHYSICS::DEBUG::PhysicsDebugRenderer _PhysicsDebugRenderer;
#else
    extern PHYSICS::DEBUG::PhysicsDebugRenderer _PhysicsDebugRenderer;
#endif

    void UpdateCamera(entt::registry& registry) {
        auto view = registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            auto& camera = view.get<CameraComponent>(entity);
            transform.Position = camera.GetCamera()->GetPosition();
            transform.Rotation = camera.GetCamera()->GetRotation();
        }
    }

    RenderSystem::RenderSystem(Renderer* renderer) {
        _Renderer = renderer;
        _Shader = ShaderLibrary::Get("core");
        _ShadowShader = ShaderLibrary::Get("shadow");
    }

    void RenderSystem::PassLightsToShader(entt::registry& registry) {}

    void RenderSystem::RenderScene(entt::registry& registry, float dt, const MEM::Ref<Shader>& shader) {
        TE_PROFILE_SCOPE("Scene Render");

        auto staticmeshview = registry.view<TransformComponent, StaticMeshComponent>();
        auto skeletalmeshview = registry.view<TransformComponent, SkeletalMeshComponent>();

        for (const auto& entity : staticmeshview) {
            const auto& transform = registry.get<TransformComponent>(entity);
            auto& static_model = registry.get<StaticMeshComponent>(entity);
            static_model.Draw(shader, transform.GetMatrix());
        }

        for (const auto& entity : skeletalmeshview) {
            auto& transform = registry.get<TransformComponent>(entity);
            auto& skeletal_model = registry.get<SkeletalMeshComponent>(entity);

            if (registry.any_of<AnimatorComponent>(entity)) {
                const auto& animator = registry.get<AnimatorComponent>(entity).GetInstance();

                animator->UpdateAnimation(dt);
                skeletal_model.GetSkeleton()->Update(animator->GetCurrentAnimationTime());
                shader->SetUniformArray("finalBonesMatrices", animator->GetFinalBoneMatrices().data(), animator->GetFinalBoneMatrices().size());
            }
            skeletal_model.Draw(shader, transform.GetMatrix());
        }
    }

    void RenderSystem::Update(entt::registry& registry, float dt, SystemUpdateMode mode)
    {
        TE_PROFILE_SCOPE("Render");

        PassLightsToShader(registry);
        auto cameraView = registry.view<TransformComponent, CameraComponent>();

        _Renderer->BeginShadowMap();
        RenderScene(registry, dt, _ShadowShader);
        _Renderer->EndShadowMap();

        for (auto& entity : cameraView) {
            CameraComponent* activeCam = &registry.get<CameraComponent>(entity);
            const ECameraType type = activeCam->GetType();;
            
            if (mode == SystemUpdateMode::GAME && type == ECameraType::PERSFECTIVE) { activeCam->GetCamera()->OnUpdate(dt); _Renderer->BeginScene(activeCam->GetCamera()); }
            else if (mode == SystemUpdateMode::EDITOR && type == ECameraType::EDITOR) { activeCam->GetCamera()->OnUpdate(dt); _Renderer->BeginScene(activeCam->GetCamera()); }
        }

        RenderScene(registry, dt, _Shader);
        _Renderer->EndScene();

        _PhysicsDebugRenderer.Flush(dt);
        Application::Get().UpdateLayers(dt);

        UpdateCamera(registry);
    }
}