#pragma once

#include <string>
#include <vector>

namespace Anwill {

    class FileIO
    {
    public:
        static std::string LoadFileToStr(const std::string& filepath);

        /**
         *
         * @param filepath - Path to the file to load
         * @param breakpoints - Gathers everything after a breakpoint substring is located
         *        (excluding the breakpoint) to the next breakpoint or until the end of
         *        the file. The gathered text is then put into a string and appended to
         *        @result.
         *        The breakpoints are used in order, so if a breakpoint isn't located,
         *        the remaining breakpoints will not be used.
         *        If the vector is empty, the entire file will be loaded into 1 string.
         * @return a vector of size equal to the size of the breakpoints param, except
         *         when it is empty.
         */
        static std::vector<std::string>
        LoadFileToStrVec(const std::string& filepath,
                         std::vector<std::string> breakpoints);
    };

}