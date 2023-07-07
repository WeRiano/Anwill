#include "Utils.h"

#include <set>
#include <cmath>
#include <sstream>
#include <iomanip>

namespace Anwill {

    unsigned int Utils::UniqueCharsInString(const std::string& str)
    {
        if (str.empty()) {
            return 0;
        }

        std::set<unsigned char> charSet;

        std::string::const_iterator c;
        for(c = str.begin(); c != str.end(); c++)
        {
            if(!charSet.contains(*c)) {
                charSet.insert(*c);
            }
        }
        return charSet.size();
    }

    std::string Utils::UniqueCharsSubstr(const std::string& str,
                                          const unsigned int maxUniqueChars)
    {
        if (maxUniqueChars <= 0 or str.empty()) {
            return str;
        }

        std::set<unsigned char> uniqueCharSet;

        std::string::const_iterator c;
        for(c = str.begin(); c != str.end(); c++)
        {
            if(!uniqueCharSet.contains(*c)) {
                uniqueCharSet.insert(*c);
                if (uniqueCharSet.size() == maxUniqueChars) {
                    return str.substr(0, maxUniqueChars);
                }
            }
        }

        return str;
    }

    std::string Utils::RoundFloatToString(float value, unsigned int decimals)
    {
        float coeff = pow(10, decimals);
        float fResult = static_cast<int>(value * coeff + 0.5f);
        fResult = static_cast<float>(fResult) / coeff;

        std::stringstream stream;
        stream << std::fixed << std::setprecision(decimals) << fResult;
        return stream.str();
    }

    int Utils::RoundToInt(float value)
    {
        return static_cast<int>(value + 0.5f);
    }
}