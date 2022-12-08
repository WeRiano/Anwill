#pragma once

#include <string>
#include <array>

#include "Vec2f.h"
#include "Vec3f.h"
#undef far
#undef near

namespace Anwill::Math {

    // Column major (it do go down, like so:)
    // -------------------
    // x.x x.y x.z transX
    // y.x y.y y.z transY
    // z.x z.y z.z transZ
    // p.x p.y p.z 1
    // -------------------
    // [x.x, y.x, z.x, p.x, x.y, y.y, z.y, p.y, x.z, y.z, z.z, p.z,
    //  transX, transY, transZ, 1]
    // -------------------
    class Mat4f
    {
    private:
        float m_Mat[4 * 4];

    public:
        static Mat4f Identity();
        static Mat4f Orthographic(float left, float right, float bottom, float top,
                                  float near, float far);
        static Mat4f Translate(const Mat4f& mat, const Vec3f& vec);
        // TODO: RotateX and RotateY or make 1 rot func
        static Mat4f Scale(const Mat4f& mat, const Vec3f& scaleVec);
        static Mat4f RotateZ(const Mat4f& mat, float deg);

        Mat4f();

        Mat4f operator*(const Mat4f& other) const;
        Vec2f operator*(const Vec2f& vec) const;
        Vec3f operator*(const Vec3f& vec) const;

        Vec3f GetTranslateVector() const;
        Vec3f GetScale() const;
        Vec3f GetRotation() const;
        std::array<Math::Vec2f, 2> Get2DBasisVectors() const;
        std::array<Math::Vec3f, 3> Get3DBasisVectors() const;
        const float* GetInternal() const;
        void SetRotZValues(const float rad);
        void SetTranslateCol(const Vec2f& tVec);
        void SetTranslateCol(const Vec3f& tVec);
        void SetTranslateCol(float tx, float ty, float tz);
        void SetIdentity();
        std::string ToString();

    private:
    };
}