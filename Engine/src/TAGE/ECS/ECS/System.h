#pragma once

#include "Components.h"
#include "entt/entt.hpp"
#include "TAGE/ECS/Objects/Object.h"
#include "TAGE/Renderer/Renderer.h"

namespace TAGE::ECS {
    enum class SystemUpdateMode {
        Editor,
        Game
    };

    class System : public Object {
    public:
        virtual ~System() = default;
        virtual void Update(entt::registry& registry, float dt, SystemUpdateMode mode) = 0;
    };

    class RenderSystem : public System {
    public:
        RenderSystem(RENDERER::Renderer* renderer);

        void Update(entt::registry& registry, float dt, SystemUpdateMode mode) override;

        void RenderScene(entt::registry& registry, float dt, const MEM::Ref<RENDERER::Shader>& shader);
        void PassLightsToShader(entt::registry& registry);
        void UpdateCamera(entt::registry& registry);
    private:
        MEM::Ref<RENDERER::Shader> _Shader;
        MEM::Ref<RENDERER::Shader> _ShadowShader;

        RENDERER::Renderer* _Renderer;
    };
}