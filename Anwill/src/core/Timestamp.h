#pragma once

#include "PlatDef.h"

#ifdef AW_PLATFORM_WINDOWS
#include <windows.h>
#endif

namespace Anwill {

    class Timestamp
    {
    public:
        Timestamp()
                : m_time(GetTime())
        {}

        Timestamp(long long time)
            : m_time(time) {}

        Timestamp& operator=(long long time) {
            this->m_time = time;
            return *this;
        }

        /**
         * Fetch absolute delta in microseconds between two timestamps.
         * @param other
         * @return abs value of this - other.
         */
        long long DeltaAbs(const Timestamp& other) const {
            return llabs(this->m_time - other.m_time);
        }

        /**
         * Fetch the delta in microseconds between two timestamps.
         * @param other
         * @return this time - the other time.
         */
        long long Delta(const Timestamp& other) const {
            return this->m_time - other.m_time;
        }

        long long operator +(long long delta) const {
            return this->m_time + delta;
        }

        void operator +=(long long delta) {
            this->m_time += delta;
        }

        long long operator -(long long delta) const {
            return this->m_time - delta;
        }

        long long operator -(const Timestamp& other) const {
            return this->m_time - other.m_time;
        }

        bool operator>(const Timestamp& other) const {
            return this->m_time > other.m_time;
        }

        bool operator>(const long long& other) const {
            return this->m_time > other;
        }

        bool operator>= (const Timestamp& other) const {
            return this->m_time >= other.m_time;
        }

        bool operator>= (const long long& other) const {
            return this->m_time >= other;
        }

        bool operator <= (const Timestamp& other) const {
            return this->m_time <= other.m_time;
        }

        bool operator <= (const long long& other) const {
            return this->m_time < other;
        }

        double GetSeconds() const
        {
            return (double)m_time / 1000000.;
        }

        double GetMilliseconds() const
        {
            return (double)m_time * 1000.;
        }

        double GetMicroseconds() const
        {
            return (double)m_time;
        }

    private:
        long long m_time; /// Microseconds

        static long long GetTime() {
            #ifdef AW_PLATFORM_WINDOWS
            LARGE_INTEGER counts_now, freq;
            QueryPerformanceCounter(&counts_now);
            QueryPerformanceFrequency(&freq);
            long long result = counts_now.QuadPart * 1000000 / freq.QuadPart;
            return result;
            #endif
        }
    };
}