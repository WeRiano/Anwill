#pragma once

#include <string>
#undef far
#undef near

namespace Anwill {

    // Column major (it do go down, like so:)
    // -------------
    // x.x x.y x.z 0
    // y.x y.y y.z 0
    // z.x z.y z.z 0
    // p.x p.y p.z 1
    // -------------
    // { x.x, y.x, z.x, p.x, x.y, y.y, z.y, p.y, x.z, y.z, z.z, p.z, transX, transY, transZ, 1 }.
    // -------------
    class Mat4f
    {
    private:
        float m_Mat[4 * 4];

        Mat4f() : m_Mat() {}
    public:
        static Mat4f Identity();
        static Mat4f Orthographic(float left, float right, float bottom, float top, float near, float far);

        Mat4f operator*(const Mat4f&);
        //Mat4f Rotate(const float deg);

        const float* GetInternal();
        std::string ToString();
    };
}