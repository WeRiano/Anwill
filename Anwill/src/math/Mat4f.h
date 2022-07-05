#pragma once

#include <string>

#include "Vec2f.h"
#include "Vec3f.h"
#undef far
#undef near

namespace Anwill::Math {

    // TODO: Implementation should probably differ based on Graphics::API?? Not sure how much so we wait with
    //       that until we get there #DirectX

    // Column major (it do go down, like so:)
    // -------------
    // x.x x.y x.z transX
    // y.x y.y y.z transY
    // z.x z.y z.z transZ
    // p.x p.y p.z 1
    // -------------
    // { x.x, y.x, z.x, p.x, x.y, y.y, z.y, p.y, x.z, y.z, z.z, p.z, transX, transY, transZ, 1 }
    // -------------
    class Mat4f
    {
    private:
        float m_Mat[4 * 4];

    public:
        static Mat4f Identity();
        static Mat4f Orthographic(float left, float right, float bottom, float top, float near, float far);

        Mat4f operator*(const Mat4f&) const;
        Vec3f operator*(const Vec3f&) const;

        const float* GetInternal() const;
        void Translate(float x, float y);
        void Translate(const Vec2f& vec);
        void Translate(float x, float y, float z);
        void Translate(const Vec3f& vec);
        Mat4f RotateZ(const float deg) const;

        std::string ToString();
    };
}