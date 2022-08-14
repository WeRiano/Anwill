#pragma once

#include "PlatDef.h"

#ifdef AW_PLATFORM_WINDOWS
#include "platform/Windows/WinTime.h"
#endif

namespace Anwill {

    class Timestamp
    {
    public:
        Timestamp()
                : m_Time(GetTime()) {}

        Timestamp(long long time)
                : m_Time(time) {}

        /**
         * Fetch absolute delta in microseconds between two timestamps.
         * @param other
         * @return abs value of this - other.
         */
        Timestamp DeltaAbs(const Timestamp& other) const
        {
            return m_Time >= other.m_Time ? m_Time - other.m_Time : other.m_Time - m_Time;
        }

        Timestamp operator+(const Timestamp& other) const
        {
            return {m_Time + other.m_Time};
        }

        void operator+=(const Timestamp& other)
        {
            m_Time += other.m_Time;
        }

        Timestamp operator-(const Timestamp& other) const
        {
            return {m_Time - other.m_Time};
        }

        void operator-=(const Timestamp& other)
        {
            m_Time -= other.m_Time;
        }

        Timestamp operator*(const long long s) const
        {
            return {m_Time * s};
        }

        void operator*=(const long long s)
        {
            m_Time *= s;
        }

        Timestamp operator/(const long long d) const
        {
            return {m_Time / d};
        }

        void operator/=(const long long d)
        {
            m_Time /= d;
        }

        bool operator>(const Timestamp& other) const
        {
            return m_Time > other.m_Time;
        }

        bool operator<(const Timestamp& other) const
        {
            return m_Time < other.m_Time;
        }

        bool operator>=(const Timestamp& other) const
        {
            return m_Time >= other.m_Time;
        }

        bool operator<=(const Timestamp& other) const
        {
            return m_Time <= other.m_Time;
        }

        bool IsZero() const
        {
            return m_Time == 0;
        }

        long double GetSeconds() const
        {
            return m_Time / 1000000.;
        }

        long double GetMilliseconds() const
        {
            return m_Time / 1000.;
        }

        long long GetMicroseconds() const
        {
            return m_Time;
        }

    private:
        long long m_Time; /// Microseconds

        static long long GetTime()
        {
            #ifdef AW_PLATFORM_WINDOWS
            return WinGetTime();
            #endif
        }
    };
}