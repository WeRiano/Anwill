#include "Profiler.h"
#include "utils/Utils.h"
#include "utils/FileIO.h"
#include "math/Math.h"
#include "core/Log.h"

namespace Anwill {

    ProfileData Profiler::s_Data;

    Profiler::Profiler(const std::string&& funcName)
        : m_FuncName(funcName), m_Start()
    {}

    Profiler::~Profiler()
    {
        Timestamp end;
        Timestamp cycleDuration = end - m_Start;
        UpdateFuncData(cycleDuration);
    }

    void Profiler::StartAppProfiling()
    {
        // totalTime is temporarily start time
        s_Data = {Timestamp()};
    }

    void Profiler::StopAppProfiling()
    {
        Timestamp now = Timestamp();
        s_Data.totalTime = now - s_Data.totalTime;
        PostAppCalc();
    }

    void Profiler::ResetData()
    {
        s_Data = {Timestamp()};
    }

    void Profiler::SaveDataToDisk()
    {
        std::string funcDataStr;
        for(std::pair<std::string, FuncProfileData> it : s_Data.funcData)
        {
            const std::string& name = it.first;
            const auto& fData = it.second;
            funcDataStr += "FUNCTION: " + name + "\n";
            funcDataStr += "MIN EXEC TIME (ms): " +
                    std::to_string(fData.cycleMinTime.GetMilliseconds()) + '\n';
            funcDataStr += "MAX EXEC TIME (ms): " +
                    std::to_string(fData.cycleMaxTime.GetMilliseconds()) + '\n';
            funcDataStr += "AVERAGE EXEC TIME (ms): " +
                    std::to_string(fData.cycleAverageTime.GetMilliseconds()) + '\n';
            funcDataStr += "NR. OF CALLS: " + std::to_string(fData.calls) + '\n';
            funcDataStr += "TOTAL LOAD (%) " +
                           std::to_string(fData.overallLoadDecimal * 100) + '\n';
            funcDataStr += "\n\n";

        }
        FileIO::StrToFile("Anwill/res/logs/profiler/FuncPerformance.txt", funcDataStr);
    }

    void Profiler::PrintDataToConsole()
    {
        for(std::pair<std::string, FuncProfileData> it : s_Data.funcData) {
            auto& fData = it.second;
            AW_INFO("Func name: {0}, Min: {1} ms, Max {2} ms, Average: {3} ms, "
                    "Overall load: {4}%, Calls: {5}",
                    it.first,
                    fData.cycleMinTime.GetMilliseconds(),
                    fData.cycleMaxTime.GetMilliseconds(),
                    fData.cycleAverageTime.GetMilliseconds(),
                    fData.overallLoadDecimal * 100,
                    fData.calls);
        }
    }

    void Profiler::UpdateFuncData(const Timestamp& duration)
    {
        FuncProfileData& fData = s_Data.funcData[m_FuncName];
        fData.totalTime += duration;
        fData.calls++;
        if (duration > fData.cycleMaxTime)
        {
            fData.cycleMaxTime = duration;
        }
        if (duration < fData.cycleMinTime)
        {
            fData.cycleMinTime = duration;
        }
    }

    void Profiler::PostAppCalc()
    {
        for(auto& it : s_Data.funcData) {
            auto& fData = it.second;
            fData.overallLoadDecimal =
                    Math::NormalizeToFloat(fData.totalTime.GetMilliseconds(),
                                           0.0l,
                                           s_Data.totalTime.GetMilliseconds());
            fData.cycleAverageTime = fData.totalTime / fData.calls;
        }
    }
}