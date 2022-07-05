#include "Mat4f.h"

#include "math/Math.h"

namespace Anwill::Math {

    Mat4f Mat4f::Identity()
    {
        // { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, transX, transY, transZ, 1 }
        Mat4f result;
        for (int i = 0; i < 16; i++)
        {
            if (i == 0 or i == 5 or i == 10 or i == 15)
            {
                result.m_Mat[i] = 1.0f;
            } else
            {
                result.m_Mat[i] = 0.0f;
            }
        }
        return result;
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

    Vec3f Mat4f::operator*(const Vec3f& vec) const
    {
        float x = vec.GetX();
        float y = vec.GetY();
        float z = vec.GetZ();
        return {m_Mat[0] * x + m_Mat[4] * y + m_Mat[8] * z,
                m_Mat[1] * x + m_Mat[5] * y + m_Mat[9] * z,
                m_Mat[2] * x + m_Mat[6] * y + m_Mat[10] * z};
    }

    const float* Mat4f::GetInternal() const
    {
        return m_Mat;
    }

    void Mat4f::Translate(float x, float y)
    {
        m_Mat[4 * 3 + 0] = x;
        m_Mat[4 * 3 + 1] = y;

    }

    void Mat4f::Translate(const Vec2f &vec)
    {
        m_Mat[4 * 3 + 0] = vec.GetX();
        m_Mat[4 * 3 + 1] = vec.GetY();
    }

    void Mat4f::Translate(float x, float y, float z)
    {
        m_Mat[4 * 3 + 0] = x;
        m_Mat[4 * 3 + 1] = y;
        m_Mat[4 * 3 + 2] = z;
    }

    void Mat4f::Translate(const Vec3f& vec)
    {
        m_Mat[4 * 3 + 0] = vec.GetX();
        m_Mat[4 * 3 + 1] = vec.GetY();
        m_Mat[4 * 3 + 2] = vec.GetZ();
    }

    Mat4f Mat4f::RotateZ(const float deg) const
    {
        float rad = DegToRad(deg);
        Mat4f rotMatZ = Identity();
        rotMatZ.m_Mat[4 * 0 + 0] = cosf(rad);
        rotMatZ.m_Mat[4 * 0 + 1] = sinf(rad);
        rotMatZ.m_Mat[4 * 1 + 0] = -sinf(rad);
        rotMatZ.m_Mat[4 * 1 + 1] = cosf(rad);
        return *this * rotMatZ;
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