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

        FuncProfileData()
            : cycleMaxTime(0), cycleMinTime(0), totalTime(0), calls(1),
              overallLoadDecimal(-1.0f), cycleAverageTime(-1)
        {}
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
        static void PostAppCalc();
    };
}

#ifdef AW_BUILD_DEBUG
#define AW_PROFILE_APP_START() Anwill::Profiler::StartAppProfiling()
#define AW_PROFILE_APP_STOP() Anwill::Profiler::StopAppProfiling()
#define AW_PROFILE_SAVE_TO_DISK() Anwill::Profiler::SaveDataToDisk()
#define AW_PROFILE_PRINT_TO_CONSOLE() Anwill::Profiler::PrintDataToConsole()

#define AW_PROFILE_FUNC_3(line) const Anwill::Profiler anwillFuncProfiler##line = \
                                Anwill::Profiler(AW_GET_FUNC_NAME)
#define AW_PROFILE_FUNC_2(line) AW_PROFILE_FUNC_3(line)
#define AW_PROFILE_FUNC() AW_PROFILE_FUNC_2(__LINE__) // __LINE__ is c++ standard, yay
#else
#define AW_PROFILE_APP_START()
#define AW_PROFILE_APP_STOP()
#define AW_PROFILE_SAVE_TO_DISK()
#define AW_PROFILE_PRINT_TO_CONSOLE()
#endif
