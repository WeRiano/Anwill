#pragma once

#include <string>

#include "core/KeyCodes.h"

// Need to use this to pass a non-static member function as a parameter.
// Member functions need to be called on an object which is passed as an implicit
// argument to the function. If you want to bind a non-static member
// function, global function or lambda, don't use this. Just pass it like normal.
#define AW_BIND_THIS_MEMBER_FUNC(fn) [this](auto&&... args) -> decltype(auto) \
{ return this->fn(std::forward<decltype(args)>(args)...); }

namespace Anwill {

    /**
     * Just random utility functions of different categories that doesn't belong anywhere
     *  specific and can be reused
     */
    class Utils
    {
    public:
        static unsigned int UniqueCharsInString(const std::string& str);

        /**
         * @brief Returns the substring of a given string after encountering a given
         *        number of unique characters.
         * @param str The string which the substring will come from.
         * @param maxUniqueChars The number of unique characters to encounter.
         * @return The substring from index 0 to whatever index the function is at when
         *         @maxUniqueChars unique characters has been encountered.
         * @return @str if there is less unique characters in @str than @maxUniqueChars,
         *              if @str is empty or if @maxUniqueChars <= 0.
         */
        static std::string UniqueCharsSubstr(const std::string& str,
                                              const unsigned int maxUniqueChars);

        static std::string RoundFloatToString(float value, unsigned int decimals);
        static int RoundToInt(float value);

        /**
         * @brief inclusive ([0, 1])
         * @tparam T
         * @param value
         * @param min
         * @param max
         * @return
         */
        template <typename T>
        static float NormalizeBetween0And1(T value, T min, T max) {
            return static_cast<float>(value - min) / static_cast<float>(max - min);
        }

        template <typename T>
        static double NormalizeBetween0And1d(T value, T min, T max) {
            return static_cast<double>(value - min) / static_cast<double>(max - min);
        }

        template <typename T>
        static float ScaleToRange(T value, T targetMin, T targetMax, T sourceMin, T sourceMax) {
            return (targetMax - targetMin) * (value - sourceMin) / (sourceMax - sourceMin) + targetMin;
        }

        template <typename T>
        static T Clamp(T value, T min, T max) {
            if(value < min) {
                return min;
            }
            if(value > max) {
                return max;
            }
            return value;
        }
    };
}
