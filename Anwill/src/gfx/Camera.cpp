#include "Camera.h"

namespace Anwill {

    Math::Mat4f Camera::GetViewProj() const
    {
        return m_ProjMat * m_ViewMat;
    }

    void Camera::SetProjMat(const Math::Mat4f&& projMat)
    {
        m_ProjMat = projMat;
    }

    const Math::Vec3f Camera::GetPos() const
    {
        return m_ViewMat.GetTranslateVector();
    }

    OrthographicCamera::OrthographicCamera(float width, float height)
    {
        // TODO: Automatically make the orthographic camera relative to the CURRENT screensize??
        //       Probably use the event system somehow ...
        //      (Subscribe to window resize event and remake m_ProjMat)
        m_ProjMat = Math::Mat4f::Orthographic(0.0f, width,
                                              0.0f, height,
                                              -1.0f, 1.0f);
        m_ViewMat = Math::Mat4f::Identity();
    }

    void OrthographicCamera::Move(float deltaX, float deltaY)
    {
        m_ViewMat = Math::Mat4f::Translate(m_ViewMat, {-deltaX, -deltaY, 0.0f});
    }
}