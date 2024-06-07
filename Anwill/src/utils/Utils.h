#pragma once

#include <string>

#include "core/KeyCodes.h"

// Need to use this to pass a non-static member function as a parameter.
// Member functions need to be called on an object which is passed as an implicit
// argument to the function. If you want to bind a non-static member
// function, global function or lambda, don't use this. Just pass it like normal.
#define AW_BIND_THIS_MEMBER_FUNC(func) [this](auto&&... args) -> decltype(auto) \
{ return this->func(std::forward<decltype(args)>(args)...); }

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

        static unsigned int GetNrOfDigits(int number);
    };
}
