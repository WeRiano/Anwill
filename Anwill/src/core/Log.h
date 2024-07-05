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
#define AW_TRACE(str, ...) WILLOG_TRACE(str, __VA_ARGS__)
#define AW_DEBUG(str, ...) WILLOG_DEBUG(str, __VA_ARGS__)
#define AW_INFO(str, ...) WILLOG_INFO(str, __VA_ARGS__)
#define AW_WARN(str, ...) WILLOG_WARN(str, __VA_ARGS__)
#define AW_ERROR(str, ...) WILLOG_ERROR(str, __VA_ARGS__)
#define AW_FATAL(str, ...) WILLOG_FATAL(str, __VA_ARGS__)
#elif defined(AW_BUILD_RELEASE)
#define AW_TRACE(str, ...)
#define AW_DEBUG(str, ...)
#define AW_INFO(str, ...)
#define AW_WARN(str, ...)
#define AW_ERROR(str, ...)
#define AW_FATAL(str, ...)
#endif