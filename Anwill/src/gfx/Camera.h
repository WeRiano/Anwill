#pragma once

#include "math/Mat4f.h"

namespace Anwill {

    class Camera
    {
    public:
        const Math::Vec3f GetPos() const;
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
        void SetProjection(float width, float height);
        float GetWidth() const;
        float GetHeight() const;

    private:
        float m_Width, m_Height;
    };
}