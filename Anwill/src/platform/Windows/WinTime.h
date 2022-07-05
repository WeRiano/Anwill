#pragma once

#include <Windows.h>

namespace Anwill {

    inline long long WinGetTime() {
        LARGE_INTEGER counts_now, freq;
        QueryPerformanceCounter(&counts_now);
        QueryPerformanceFrequency(&freq);
        long long result = counts_now.QuadPart * 1000000 / freq.QuadPart;
        return result;
    }
}