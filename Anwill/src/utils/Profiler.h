#include <string>
#include <unordered_map>
#include <string>

#include "core/Timestamp.h"

#pragma once

namespace Anwill {

    struct FuncProfileData {
        Timestamp cycleMaxTime;
        Timestamp cycleMinTime;
        Timestamp totalTime;
        unsigned long long calls;

        // Members that will be defined during app termination
            // What percentage of the total runtime was spent in this func? [0.0, 1.0]
        float overallLoadDecimal;
        Timestamp cycleAverageTime;
    };

    struct ProfileData {
        Timestamp totalTime;
        std::unordered_map<std::string, FuncProfileData> funcData;
    };

    class Profiler
    {
    public:
        static void StartAppProfiling();
        static void StopAppProfiling();
        static void ResetData();
        static void SaveDataToDisk();
        // Usage of this should probably be limited to testing
        static void PrintDataToConsole();

        Profiler(const std::string&& funcName);
        ~Profiler();

    private:
        static ProfileData s_Data;

        std::string m_FuncName;
        Timestamp m_Start;

        void UpdateFuncData(const Timestamp& duration);
        static void PostTerminationCalc();
    };
}

#ifdef AW_BUILD_DEBUG
#define AW_PROFILE_FUNC() Anwill::Profiler(AW_GET_FUNC_NAME);
#else
#define AW_PROFILE_FUNC()
#endif
