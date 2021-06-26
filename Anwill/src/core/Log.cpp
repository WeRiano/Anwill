#include "Log.h"

namespace Anwill {

    void Log::Init()
    {
        Willog::Init();

        Willog::SetLogLevel(Willog::LogLevel::ALL);
        Willog::ShowFP();
        Willog::ShowLine();
    }

}