#pragma once

#include "Raycaster.h"
#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

namespace TAGE::PHYSICS::DEBUG {
    class PhysicsDebugRenderer : public btIDebugDraw
    {
    public:
        void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
        void drawLineForSeconds(const btVector3& from, const btVector3& to, const btVector3& color, float duration);
        void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB,
            btScalar distance, int lifeTime, const btVector3& color) override;
        void reportErrorWarning(const char* warningString) override;
        void draw3dText(const btVector3& location, const char* textString) override;
        void setDebugMode(int debugMode) override { m_DebugMode = debugMode; }
        int getDebugMode() const override { return m_DebugMode; }

        void Flush(float dt);

    private:
        std::vector<RAYCAST::RaycastLine> m_Lines;
        int m_DebugMode = DBG_DrawWireframe | DBG_DrawContactPoints | DBG_DrawAabb;
    };
}