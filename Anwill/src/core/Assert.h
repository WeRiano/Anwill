#pragma once

#include "core/PlatDef.h"
#include "core/Log.h"

#define AW_EXPAND_MACRO(x) x
#define AW_ASSERT_MSG(check, msg) if(!(check)) { AW_FATAL(msg); __debugbreak(); } // TODO: Should this be a FATAL log?
#define AW_ASSERT_EMPTY(check) if(check) { __debugbreak(); }
// If no msg is given, then _2 is AW_ASSERT_MSG, which makes NAME AW_ASSERT_EMPTY.
// If msg is given, _1 and _2 is check and msg respectively, and name becomes AW_ASSERT_MSG. Smart!
#define AW_ASSERT_GET_MACRO(_1, _2, NAME, ...) NAME

#if defined(AW_BUILD_DEBUG)
#define AW_ASSERT(...) AW_EXPAND_MACRO(AW_ASSERT_GET_MACRO(__VA_ARGS__, AW_ASSERT_MSG, AW_ASSERT_EMPTY)(__VA_ARGS__))
#elif defined(AW_BUILD_RELEASE)
#define AW_ASSERT(...)
#endif
