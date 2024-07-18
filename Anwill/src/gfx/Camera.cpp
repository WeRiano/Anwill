#include "Camera.h"

namespace Anwill {

    const Math::Vec3f Camera::GetPos() const
    {
        return m_ViewMat.GetTranslateVector();
    }

    Math::Mat4f Camera::GetViewProj() const
    {
        return m_ProjMat * m_ViewMat;
    }

    OrthographicCamera::OrthographicCamera(float width, float height)
        : m_Width(width), m_Height(height)
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

    void OrthographicCamera::Move(const Math::Vec2f delta)
    {
        m_ViewMat = Math::Mat4f::Translate(m_ViewMat, delta);
    }

    void OrthographicCamera::SetPos(float x, float y)
    {
        m_ViewMat.SetTranslation(x, y, 0.0f);
    }

    void OrthographicCamera::SetPos(const Math::Vec2f pos)
    {
        m_ViewMat.SetTranslation(pos);
    }

    void OrthographicCamera::SetProjection(float width, float height)
    {
        m_ProjMat = Math::Mat4f::Orthographic(0.0f, width, 0.0f, height, -1.0f, 1.0f);
    }

    float OrthographicCamera::GetWidth() const
    {
        return m_Width;
    }

    float OrthographicCamera::GetHeight() const
    {
        return m_Height;
    }
}