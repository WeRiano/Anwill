#pragma once

#include <string>

#define AW_MATH_PI 3.14159265359

namespace Anwill::Math {

    float DegToRad(float deg);
    float RadToDeg(float rad);
    std::string RoundToString(float value, unsigned int decimals);
    int RoundToInt(float value);

    /**
     * @brief Normalize value to the range [0, 1]. Also called rescaling (min-max normalization).
     * @tparam T Numeric type of all parameters.
     * @param value Value to normalize.
     * @param min Source range minimum.
     * @param max Source range maximum.
     * @return Normalized value on the range [0, 1] as a float.
     */
    template <typename T>
    float NormalizeToFloat(T value, T min, T max)
    {
        return static_cast<float>(value - min) / static_cast<float>(max - min);
    }

    /**
     * @brief Normalize value to the range [0, 1]. Also called rescaling (min-max normalization).
     * @tparam T Numeric type of all parameters.
     * @param value Value to normalize.
     * @param min Source range minimum.
     * @param max Source range maximum.
     * @return Normalized value on the range [0, 1] as a double.
     */
    template <typename T>
    double NormalizeToDouble(T value, T min, T max)
    {
        return static_cast<double>(value - min) / static_cast<double>(max - min);
    }

    template <typename T>
    T ScaleToRange(T value, T targetMin, T targetMax, T sourceMin, T sourceMax)
    {
        return (targetMax - targetMin) * (value - sourceMin) / (sourceMax - sourceMin) + targetMin;
    }

    template <typename T>
    T Clamp(T value, T min, T max)
    {
        return min * (value < min) + max * (value > max) + (value >= min && value <= max) * value;
    }

    template <typename T>
    T Max(T value1, T value2)
    {
        return value1 * (value1 >= value2) + value2 * (value1 < value2);
    }

    template <typename T>
    T Min(T value1, T value2)
    {
        return value1 * (value1 <= value2) + value2 * (value1 > value2);
    }

    template <typename T>
    T Abs(T value)
    {
        return value * ((value > 0) - (value < 0));
    }
}
