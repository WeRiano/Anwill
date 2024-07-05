#include <fstream>
#include <iostream>

#include "FileIO.h"
#include "core/Assert.h"

namespace Anwill {

    std::string FileIO::LoadFileToStr(const std::string& filepath)
    {
        std::string result;
        std::ifstream file(filepath);
        if (file.is_open())
        {
            std::string line;
            while(std::getline(file, line))
            {
                //std::cout << line << '\n';
                result += line + "\n";
            }
            result.erase(result.length() - 1, 2);
            file.close();
        } else {
            AW_ERROR("Could not load the following file: {0}", filepath);
        }
        return result;
    }

    std::vector<std::string> FileIO::LoadFileToStrVec(const std::string& filepath,
                                            std::vector<std::string> breakpoints)
    {
        std::vector<std::string> result;
        if(breakpoints.empty())
        {
            result.push_back(LoadFileToStr(filepath));
            return result;
        }

        bool foundFirst = false;
        unsigned int bpIndex = 0;
        std::string curStr = "";
        std::ifstream file(filepath);
        if (file.is_open())
        {
            std::string line;
            while(std::getline(file, line))
            {
                if(breakpoints.size() > bpIndex and
                   line.find(breakpoints[bpIndex]) != std::string::npos)
                {
                    std::getline(file, line);
                    bpIndex++;
                    if(!foundFirst)
                    {
                        foundFirst = true;
                    } else {
                        result.push_back(curStr);
                        curStr = "";
                    }
                }
                if(foundFirst)
                {
                    curStr += line + '\n';
                }
            }
            result.push_back(curStr);
            file.close();
        } else {
            AW_ERROR("Could not load the following file: {0}", filepath);
        }
        return result;
    }

    void FileIO::StrToFile(const std::string& filePathAndName, const std::string& str)
    {
        std::ofstream file = std::ofstream(filePathAndName);
        file << str;
        file.close();
    }
}