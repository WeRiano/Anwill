#include "Math.h"

#include <iostream>
#include <sstream>
#include <iomanip>

namespace Anwill::Math {

    float DegToRad(float deg)
    {
        //return deg * AW_MATH_PI / 180.0f;
        return deg * 0.01745329251f;
    }

    float RadToDeg(float rad)
    {
        //return rad * 180.0f / AW_MATH_PI;
        return rad * 57.2957795131f;
    }

    std::string RoundToString(float value, unsigned int decimals)
    {
        float coeff = pow(10, decimals);
        float fResult = static_cast<int>(value * coeff + 0.5f);
        fResult = static_cast<float>(fResult) / coeff;

        std::stringstream stream;
        stream << std::fixed << std::setprecision(decimals) << fResult;
        return stream.str();
    }

    int RoundToInt(float value)
    {
        return static_cast<int>(value + 0.5f);
    }
}