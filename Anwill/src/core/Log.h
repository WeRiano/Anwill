#pragma once

#include "Willog.h"

class Log
{
    static void Init();
};

#define AW_TRACE(x) Willog::Trace(x);
#define AW_DEBUG(x) Willog::Debug(x);
#define AW_INFO(x) Willog::Info(x);
#define AW_WARN(x) Willog::Warn(x);
#define AW_ERROR(x) Willog::Error(x);
#define AW_FATAL(x) Willog::Fatal(x);