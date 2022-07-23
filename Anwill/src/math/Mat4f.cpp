#include "Mat4f.h"

#include "math/Math.h"

namespace Anwill::Math {

    Mat4f Mat4f::Identity()
    {
        Mat4f res;
        res.SetIdentity();
        return res;
    }

    Mat4f Mat4f::Orthographic(float left, float right, float bottom, float top, float near, float far)
    {
        Mat4f result = Identity();
        result.m_Mat[4 * 0 + 0] = 2.0f / (right - left);
        result.m_Mat[4 * 1 + 1] = 2.0f / (top - bottom);
        result.m_Mat[4 * 2 + 2] = -2.0f / (far - near);

        result.m_Mat[4 * 3 + 0] = -(right + left) / (right - left);
        result.m_Mat[4 * 3 + 1] = -(top + bottom) / (top - bottom);
        result.m_Mat[4 * 3 + 2] = -(far + near) / (far - near);
        result.m_Mat[4 * 3 + 3] = 1.0f;
        return result;
    }

    Mat4f Mat4f::Translate(const Mat4f& mat, const Vec3f& vec)
    {
        Mat4f translateMat = Identity();
        translateMat.SetTranslateCol(vec);
        return translateMat * mat;
    }

    Mat4f Mat4f::Scale(const Mat4f& mat, const Vec3f& scaleVec)
    {
        Mat4f result = mat;
        result.m_Mat[0] *= scaleVec.GetX();
        result.m_Mat[5] *= scaleVec.GetY();
        result.m_Mat[10] *= scaleVec.GetZ();
        return result;
    }

    Mat4f Mat4f::RotateZ(const Mat4f& mat, const float deg)
    {
        float rad = DegToRad(deg);
        Mat4f rotMat = Identity();
        rotMat.SetRotZValues(rad);
        return rotMat * mat;
    }

    Mat4f::Mat4f()
    {
        SetIdentity();
    }

    Mat4f Mat4f::operator*(const Mat4f &other) const
    {
        Mat4f result = Identity();
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                float sum = 0;
                for (int k = 0; k < 4; k++)
                {
                    sum += m_Mat[i + k * 4] * other.m_Mat[k + j * 4];
                }
                result.m_Mat[i + j * 4] = sum;
            }
        }
        return result;
    }

    Vec2f Mat4f::operator*(const Vec2f& vec) const
    {
        float x = vec.GetX();
        float y = vec.GetY();
        return { m_Mat[0] * x + m_Mat[4] * y + m_Mat[12] * 1,
                 m_Mat[1] * x + m_Mat[5] * y + m_Mat[13] * 1};
    }

    Vec3f Mat4f::operator*(const Vec3f& vec) const
    {
        float x = vec.GetX();
        float y = vec.GetY();
        float z = vec.GetZ();
        return {m_Mat[0] * x + m_Mat[4] * y + m_Mat[8] * z + m_Mat[12],
                m_Mat[1] * x + m_Mat[5] * y + m_Mat[9] * z + m_Mat[13],
                m_Mat[2] * x + m_Mat[6] * y + m_Mat[10] * z + m_Mat[14]};
    }

    Vec3f Mat4f::GetTranslateVector() const
    {
        return {m_Mat[12], m_Mat[13], m_Mat[14]};
    }

    std::array<Math::Vec2f, 2> Mat4f::Get2DBasisVectors() const
    {
        return { Vec2f(m_Mat[0], m_Mat[1]), Vec2f(m_Mat[4], m_Mat[5])};
    }

    std::array<Math::Vec3f, 3> Mat4f::Get3DBasisVectors() const
    {
        return { Vec3f(m_Mat[0], m_Mat[1], m_Mat[2]),
                 Vec3f(m_Mat[4], m_Mat[5], m_Mat[6]),
                 Vec3f(m_Mat[8], m_Mat[9], m_Mat[10])};
    }

    const float* Mat4f::GetInternal() const
    {
        return m_Mat;
    }

    void Mat4f::SetRotZValues(const float rad)
    {
        m_Mat[4 * 0 + 0] = cosf(rad);
        m_Mat[4 * 0 + 1] = sinf(rad);
        m_Mat[4 * 1 + 0] = -sinf(rad);
        m_Mat[4 * 1 + 1] = cosf(rad);
    }

    void Mat4f::SetTranslateCol(const Vec2f& tVec)
    {
        m_Mat[4 * 3 + 0] = tVec.GetX();
        m_Mat[4 * 3 + 1] = tVec.GetY();
    }

    void Mat4f::SetTranslateCol(const Vec3f& tVec)
    {
        m_Mat[4 * 3 + 0] = tVec.GetX();
        m_Mat[4 * 3 + 1] = tVec.GetY();
        m_Mat[4 * 3 + 2] = tVec.GetZ();
    }

    void Mat4f::SetTranslateCol(float tx, float ty, float tz)
    {
        m_Mat[4 * 3 + 0] = tx;
        m_Mat[4 * 3 + 1] = ty;
        m_Mat[4 * 3 + 2] = tz;
    }

    void Mat4f::SetIdentity()
    {
        m_Mat[0] = 1.0f;
        m_Mat[1] = 0.0f;
        m_Mat[2] = 0.0f;
        m_Mat[3] = 0.0f;
        m_Mat[4] = 0.0f;
        m_Mat[5] = 1.0f;
        m_Mat[6] = 0.0f;
        m_Mat[7] = 0.0f;
        m_Mat[8] = 0.0f;
        m_Mat[9] = 0.0f;
        m_Mat[10] = 1.0f;
        m_Mat[11] = 0.0f;
        m_Mat[12] = 0.0f;
        m_Mat[13] = 0.0f;
        m_Mat[14] = 0.0f;
        m_Mat[15] = 1.0f;
    }

    std::string Mat4f::ToString()
    {
        std::string result;
        for (unsigned int i = 0; i < 4; i++)
        {
            for (unsigned int j = 0; j < 4; j++)
            {
                result += std::to_string(m_Mat[4 * i + j]);
                result += " ";
            }
            result += "\n";
        }
        return result;
    }
}