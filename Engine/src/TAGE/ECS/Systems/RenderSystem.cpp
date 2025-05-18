#include "tagepch.h"
#include "System.h"
#include "TAGE/Renderer/Shader/ShaderLibrary.h"
#include "TAGE/Core/Application/Application.h"
#include "TAGE/Renderer/Optimization/FrustumCulling.h"
#include "TAGE/Renderer/Model/Animation/AnimInstance.h"

namespace TAGE::ECS{
#ifndef PHYSICS_DEBUGER_INCLUDED
    PHYSICS::DEBUG::PhysicsDebugRenderer _PhysicsDebugRenderer;
#else
    extern PHYSICS::DEBUG::PhysicsDebugRenderer _PhysicsDebugRenderer;
#endif
    RenderSystem::RenderSystem() {
    }

    void RenderSystem::Init()
	{
		_Renderer = &Application::Get().GetRenderer();
        _Shader = ShaderLibrary::Get("core");
        _ShadowShader = ShaderLibrary::Get("shadow");
    }

    void UpdateCamera(entt::registry& registry) {
        auto view = registry.view<TransformComponent, CameraComponent>();
        for (auto entity : view) {
            auto& transform = view.get<TransformComponent>(entity);
            auto& camera = view.get<CameraComponent>(entity);
            transform.Position = camera.GetCamera()->GetPosition();
            transform.Rotation = camera.GetCamera()->GetRotation();
        }
    }

    void RenderSystem::Update(float dt)
    {
        if (auto scene = _Scene.lock()) {
            TE_PROFILE_SCOPE("Render");
            const auto registry = scene->GetRegistry().GetRegistry();

            PassLightsToShader();
            auto cameraView = registry->view<TransformComponent, CameraComponent>();

            _Renderer->BeginShadowMap();
            RenderScene(dt, _ShadowShader);
            _Renderer->EndShadowMap();
            
            auto& camMgr = GAMEFACTORY::GameInstance::Get()->GetCameraManager();
            if (camMgr.HasActiveCamera())
            {
				auto activeCam = camMgr.GetActiveCamera();
                activeCam->Update();
				_Renderer->BeginScene(activeCam);
			}

            RenderScene(dt, _Shader);
            _PhysicsDebugRenderer.Flush(dt);
            _Renderer->EndScene();

            Application::Get().UpdateLayers(dt);

            UpdateCamera(*registry);
        }
    }

    void RenderSystem::RenderScene(float dt, const MEM::Ref<Shader>& shader) {
        if (auto scene = _Scene.lock()) {
            TE_PROFILE_SCOPE("Scene Render");
            const auto registry = scene->GetRegistry().GetRegistry();

            auto viewProj = Renderer::_SceneData.viewProjectionMatrix;
            auto frustum = FRUSTUM::ExtractFrustumPlanes(viewProj);

            auto isInFrustum = [&](const glm::mat4& matrix, const glm::vec3& localCenter, float localRadius) -> bool {
                glm::vec4 worldCenter4 = matrix * glm::vec4(localCenter, 1.0f);
                glm::vec3 worldCenter = glm::vec3(worldCenter4) / worldCenter4.w;
                float maxScale = glm::compMax(glm::abs(glm::vec3(matrix[0][0], matrix[1][1], matrix[2][2])));
                float worldRadius = localRadius * maxScale;
                return FRUSTUM::IsSphereInFrustum(worldCenter, worldRadius, frustum);
                };

            auto staticmeshview = registry->view<TransformComponent, StaticMeshComponent>();
            staticmeshview.each([&](auto entity, const TransformComponent& transform, StaticMeshComponent& static_model) {
                auto model = static_model.GetModel();

                shader->SetUniform("u_PlayAnimation", false);
                static_model.Draw(shader, transform.GetMatrix());
                });

            auto SkeletalView = registry->view<TransformComponent, SkeletalMeshComponent>();
            SkeletalView.each([&](auto entity, TransformComponent& transform, SkeletalMeshComponent& skeletal_model) {
                auto model = skeletal_model.GetModel();
                bool playAnimation = false;

                if (auto skeleton = skeletal_model.GetSkeleton()) {
                    if (auto animInstance = skeleton->GetAnimInstance()) {
                        animInstance->UpdateAnimInstance(dt);
                        if (auto* animator = animInstance->GetAnimator().GetInstance()) {
                            shader->SetUniformArray("finalBonesMatrices",
                                animator->GetFinalBoneMatrices().data(),
                                animator->GetFinalBoneMatrices().size());
                            playAnimation = true;
                        }
                    }
                }

                shader->SetUniform("u_PlayAnimation", playAnimation);
                skeletal_model.Draw(shader, transform.GetMatrix());
                });
        }
    }

    void RenderSystem::PassLightsToShader() {}
}