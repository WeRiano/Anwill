#include "Camera.h"
#include "events/SystemEventHandler.h"
#include "events/WindowEvents.h"
#include "math/Math.h"

namespace Anwill {

    Camera::Camera(const Math::Mat4f& projMat)
        : m_ProjMat(projMat)
    {}

    Math::Vec3f Camera::GetPos() const
    {
        return m_ViewMat.GetTranslateVector();
    }

    Math::Mat4f Camera::GetViewProj() const
    {
        return m_ProjMat * m_ViewMat;
    }

    OrthographicCamera::OrthographicCamera(float width, float height)
        : Camera(GetProjection(width, height)), m_Width(width), m_Height(height), m_ZoomLevel(0)
    {
        m_ViewMat = Math::Mat4f::Identity();

        //SystemEventHandler::Subscribe<WindowResizeEvent>(AW_BIND_THIS_MEMBER_FUNC(OnWindowResize));
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

    void OrthographicCamera::Zoom(float delta)
    {
        m_ZoomLevel = Math::Clamp(m_ZoomLevel + delta, -GetMaxZoom(), GetMaxZoom());
        float oldAspectRatio = m_Width / m_Height;
        m_ProjMat = GetProjection((m_Height + m_ZoomLevel) * oldAspectRatio, m_Height + m_ZoomLevel);
    }

    void OrthographicCamera::SetProjection(float width, float height)
    {
        m_Width = width;
        m_Height = height;
        float oldAspectRatio = m_Width / m_Height;
        m_ProjMat = GetProjection((m_Height + m_ZoomLevel) * oldAspectRatio, m_Height + m_ZoomLevel);
    }

    Math::Mat4f OrthographicCamera::GetProjection(float width, float height) const
    {
        float spanX = width * 0.5f;
        float spanY = height * 0.5f;
        return Math::Mat4f::Orthographic(-spanX, spanX, -spanY, spanY, -1.0f, 1.0f);
    }

    float OrthographicCamera::GetMaxZoom() const
    {
        return m_Height * 0.75f;
    }

    void OrthographicCamera::OnWindowResize(Unique<Event>& event)
    {
        const auto& e = static_cast<WindowResizeEvent&>(*event);
        m_Width = (float)e.GetNewWidth();
        m_Height = (float)e.GetNewHeight();
        float oldAspectRatio = m_Width / m_Height;
        SetProjection((m_Height + m_ZoomLevel) * oldAspectRatio, m_Height + m_ZoomLevel);
    }
}