#pragma once

#include "PlatDef.h"
#include "Willog.hpp"

namespace Anwill {

    class Log
    {
    public:
        static void Init();
    };

}

#if defined(AW_BUILD_DEBUG)
#define AW_TRACE(s, ...) WILLOG_TRACE(s, __VA_ARGS__)
#define AW_DEBUG(s, ...) WILLOG_DEBUG(s, __VA_ARGS__)
#define AW_INFO(s, ...) WILLOG_INFO(s, __VA_ARGS__)
#define AW_WARN(s, ...) WILLOG_WARN(s, __VA_ARGS__)
#define AW_ERROR(s, ...) WILLOG_ERROR(s, __VA_ARGS__)
#define AW_FATAL(s, ...) WILLOG_FATAL(s, __VA_ARGS__)
#elif defined(AW_BUILD_RELEASE)
#define AW_TRACE(s, ...)
#define AW_DEBUG(s, ...)
#define AW_INFO(s, ...)
#define AW_WARN(s, ...)
#define AW_ERROR(s, ...)
#define AW_FATAL(s, ...)
#endif