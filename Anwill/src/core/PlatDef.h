#pragma once

#if defined(_WIN32)
    //#define AW_PLATFORM_WINDOWS_32
    #if defined(_WIN64)
        #define AW_PLATFORM_WINDOWS
        #if defined(_DEBUG)
            #define AW_BUILD_DEBUG
        #else
            #define AW_BUILD_RELEASE
        #endif
    #else
        #error "Win32 not supported."
    #endif
#else
    #error "Anwill does not currently support this platform.
#endif

#if defined(__FUNCSIG__) || (_MSC_VER)
    #define AW_GET_FUNC_NAME __FUNCTION__
    #define AW_GET_FUNC_SIG __FUNCSIG__
#else
    #error "Anwill does not currently support this compiler.
#endif