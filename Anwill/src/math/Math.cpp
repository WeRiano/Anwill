#include "Math.h"

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
}