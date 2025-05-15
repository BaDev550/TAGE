#include "tagepch.h"
#include "System.h"
#include "TAGE/Renderer/Shader/ShaderLibrary.h"
#include "TAGE/Core/Application/Application.h"
#include "TAGE/Renderer/Optimization/FrustumCulling.h"

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

        auto viewProj = Renderer::GetSceneData().viewProjectionMatrix;
        auto frustum = FRUSTUM::ExtractFrustumPlanes(viewProj);

        auto isInFrustum = [&](const glm::mat4& matrix, const glm::vec3& localCenter, float localRadius) -> bool {
            glm::vec4 worldCenter4 = matrix * glm::vec4(localCenter, 1.0f);
            glm::vec3 worldCenter = glm::vec3(worldCenter4) / worldCenter4.w;
            float maxScale = glm::compMax(glm::abs(glm::vec3(matrix[0][0], matrix[1][1], matrix[2][2])));
            float worldRadius = localRadius * maxScale;
            return FRUSTUM::IsSphereInFrustum(worldCenter, worldRadius, frustum);
            };

        auto staticmeshview = registry.view<TransformComponent, StaticMeshComponent>();
        staticmeshview.each([&](auto entity, const TransformComponent& transform, StaticMeshComponent& static_model) {
            auto model = static_model.GetModel();
             
            if (!isInFrustum(transform.GetMatrix(), model->GetCenterOffset(), model->GetBoundingRadius()))
                return;

            static_model.Draw(shader, transform.GetMatrix());
            });

        auto animatedSkeletalView = registry.view<TransformComponent, SkeletalMeshComponent, AnimatorComponent>();
        animatedSkeletalView.each([&](auto entity, TransformComponent& transform, SkeletalMeshComponent& skeletal_model, AnimatorComponent& animatorComp) {
            auto model = skeletal_model.GetModel();

            if (!isInFrustum(transform.GetMatrix(), model->GetCenterOffset(), model->GetBoundingRadius()))
                return;

            auto* animator = animatorComp.GetInstance();
            animator->UpdateAnimation(dt);
            skeletal_model.GetSkeleton()->Update(animator->GetCurrentAnimationTime());
            shader->SetUniformArray("finalBonesMatrices", animator->GetFinalBoneMatrices().data(), animator->GetFinalBoneMatrices().size());
            skeletal_model.Draw(shader, transform.GetMatrix());
            });

        auto nonAnimatedSkeletalView = registry.view<TransformComponent, SkeletalMeshComponent>(entt::exclude<AnimatorComponent>);
        nonAnimatedSkeletalView.each([&](auto entity, TransformComponent& transform, SkeletalMeshComponent& skeletal_model) {
            auto model = skeletal_model.GetModel();

            if (!isInFrustum(transform.GetMatrix(), model->GetCenterOffset(), model->GetBoundingRadius()))
                return;

            skeletal_model.Draw(shader, transform.GetMatrix());
            });
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
            
            if (mode == SystemUpdateMode::GAME && type == ECameraType::PERSFECTIVE) {   _Renderer->BeginScene(activeCam->GetCamera()); }
            else if (mode == SystemUpdateMode::EDITOR && type == ECameraType::EDITOR) { _Renderer->BeginScene(activeCam->GetCamera()); }
        }

        RenderScene(registry, dt, _Shader);
        _Renderer->EndScene();

        _PhysicsDebugRenderer.Flush(dt);
        Application::Get().UpdateLayers(dt);

        UpdateCamera(registry);
    }
}