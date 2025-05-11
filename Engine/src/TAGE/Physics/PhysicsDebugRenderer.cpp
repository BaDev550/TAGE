#include "tagepch.h"
#include "TAGE/Renderer/Renderer.h"
#include "Raycaster.h"
#include "PhysicsDebugRenderer.h"

namespace TAGE::PHYSICS::DEBUG {
    void PhysicsDebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
    {
        m_Lines.push_back({
            glm::vec3(from.x(), from.y(), from.z()),
            glm::vec3(to.x(), to.y(), to.z()),
            glm::vec3(color.x(), color.y(), color.z()),
            RAYCAST::RayDrawType::FOR_FRAME,
            0.0f
        });
    }

    void PhysicsDebugRenderer::drawLineForSeconds(const btVector3& from, const btVector3& to, const btVector3& color, float duration)
    {
        m_Lines.push_back({
            glm::vec3(from.x(), from.y(), from.z()),
            glm::vec3(to.x(), to.y(), to.z()),
            glm::vec3(color.x(), color.y(), color.z()),
            RAYCAST::RayDrawType::FOR_DURATION,
            duration
        });
    }

    void PhysicsDebugRenderer::drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB,
        btScalar distance, int lifeTime, const btVector3& color)
    {
        btVector3 to = pointOnB + normalOnB * distance;
        drawLine(pointOnB, to, color);
    }

    void PhysicsDebugRenderer::reportErrorWarning(const char* warningString)
    {
        CORE_LOG_WARN("Bullet Debug: {0}", warningString);
    }

    void PhysicsDebugRenderer::draw3dText(const btVector3& location, const char* textString) { /* optional */ }

    void PhysicsDebugRenderer::Flush(float dt)
    {
        auto it = m_Lines.begin();
        while (it != m_Lines.end()) {
            RENDERER::DEBUG::DebugRenderer::DrawLine(it->from, it->to, it->color);

            if (it->drawType == RAYCAST::RayDrawType::FOR_DURATION) {
                it->time -= dt;
                if (it->time <= 0.0f) {
                    it = m_Lines.erase(it);
                    continue;
                }
            }
            else if (it->drawType == RAYCAST::RayDrawType::FOR_FRAME) {
                it = m_Lines.erase(it);
                continue;
            }

            ++it;
        }
    }
}