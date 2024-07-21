#pragma once

#include "core/Core.h"
#include "math/Mat4f.h"
#include "events/Event.h"

namespace Anwill {

    class Camera
    {
    public:
        Camera(const Math::Mat4f& projMat);

        Math::Vec3f GetPos() const;
        Math::Mat4f GetViewProj() const;

    protected:
        Math::Mat4f m_ViewMat;
        Math::Mat4f m_ProjMat; // This depends on derived type
    };

    class OrthographicCamera : public Camera
    {
    public:
        OrthographicCamera(float width, float height);

        // positive x values means moving the camera to the right, and vise-versa
        // positive y values means moving the camera up, and vise-versa
        void Move(float deltaX, float deltaY);
        void Move(const Math::Vec2f delta);
        void SetPos(float x, float y);
        void SetPos(const Math::Vec2f pos);
        void Zoom(float delta);
        void SetProjection(float width, float height);

    protected:
        float m_Width, m_Height;
        float m_ZoomLevel;

        Math::Mat4f GetProjection(float width, float height) const;
        float GetMaxZoom() const;
        void OnWindowResize(Unique<Event>& event);
    };
}