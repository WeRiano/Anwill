#pragma once

#include <cmath>

#include "PlatDef.h"

#ifdef AW_PLATFORM_WINDOWS
#include "platform/Windows/WinTime.h"
#endif

namespace Anwill {

    /**
     * Represents a point in time or a duration.
     */
    class Timestamp
    {
    public:
        enum class Unit : long long {
            Seconds = 6,
            Milliseconds = 3,
            Microseconds = 0,
            Nanoseconds = -3
        };

        /*
        static Timestamp CreateTimestampInSeconds();
        static Timestamp CreateTimestampInMilliseconds();
        static Timestamp CreateTimestampInMicroseconds();
         */

        Timestamp()
                : m_Time(GetTime()) {}

        Timestamp(long long microseconds)
                : m_Time(microseconds) {}

        Timestamp(long long value, const Unit&& unit)
        {
            switch(unit) {
                case Unit::Seconds:
                    m_Time = value * 1000000;
                    break;
                case Unit::Milliseconds:
                    m_Time = value * 1000;
                    break;
                case Unit::Microseconds:
                    m_Time = value;
                    break;
                case Unit::Nanoseconds:
                    m_Time = value / 1000;
                    break;

            }

            m_Time = static_cast<long long>(
                    static_cast<long double>(value) * std::powl(10.0l, static_cast<long double>(unit))
            );
        }

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

        Timestamp operator*(const double s) const
        {
            return {static_cast<long long>(static_cast<double long>(m_Time) * s)};
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
            return static_cast<long double>(m_Time) / 1000000.;
        }

        long double GetMilliseconds() const
        {
            return static_cast<long double>(m_Time) / 1000.;
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