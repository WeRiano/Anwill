#pragma once

#include "Timestamp.h"

namespace Anwill {

    class Layer
    {
    public:
        Layer(const unsigned int ups)
                : m_UpdateDelta(ups == 0 ? 0 : 1000000/ups), m_LastUpdate(Timestamp())
        {}

        virtual void Update(const Timestamp& timestamp) {
            m_LastUpdate = timestamp;
        }

        const Timestamp GetUpdateDelta() {
            return m_UpdateDelta;
        }

        const Timestamp GetLastUpdate() {
            return m_LastUpdate;
        }

    protected:
        Timestamp m_UpdateDelta, m_LastUpdate;
    };
}