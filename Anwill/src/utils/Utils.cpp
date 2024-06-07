#include "core/Input.h"
#include "utils/Utils.h"

#include <set>
#include <sstream>

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

    unsigned int Utils::GetNrOfDigits(int number)
    {
        unsigned int result = 0;
        while(number)
        {
            result++;
            number /= 10;
        }
        return result;
    }
}