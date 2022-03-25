#include "Mat4f.h"

namespace Anwill {

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
        Mat4f result;
        result.m_Mat[4 * 0 + 0] = 2.0f / right - left;
        result.m_Mat[4 * 1 + 1] = 2.0f / top - bottom;
        result.m_Mat[4 * 2 + 2] = 2.0f / far - near;

        result.m_Mat[4 * 3 + 0] = -1.0f * ((right + left) / (right - left));
        result.m_Mat[4 * 3 + 1] = -1.0f * ((top + bottom) / (top - bottom));
        result.m_Mat[4 * 3 + 2] = -1.0f * ((far + near) / (far - near));
        result.m_Mat[4 * 3 + 3] = 1.0f;
        return result;
    }

    Mat4f Mat4f::operator*(const Mat4f& other)
    {
        Mat4f result;
        for (int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                float sum = 0;
                for(int k = 0; k < 4; k++)
                {
                    sum += m_Mat[i + k * 4] * other.m_Mat[k + j * 4];
                }
                result.m_Mat[i + j * 4] = sum;
            }
        }
        return result;
    }

    const float* Mat4f::GetInternal()
    {
        return m_Mat;
    }

    std::string Mat4f::ToString()
    {
        std::string result;
        for(unsigned int i = 0; i < 4; i++)
        {
            for(unsigned int j = 0; j < 4; j++)
            {
                result += std::to_string(m_Mat[4 * i + j]);
                result += " ";
            }
            result += "\n";
        }
        return result;
    }
}