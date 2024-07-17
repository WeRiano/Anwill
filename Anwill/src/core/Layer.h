#pragma once

#include "Timestamp.h"

namespace Anwill {

    class Layer
    {
    public:
        explicit Layer(unsigned int ups)
                : m_UpdateDelta(ups == 0 ? 0 : 1000000/ups), m_LastUpdate(Timestamp())
        {}

        virtual void Update(const Timestamp& timestamp) {
            m_LastUpdate = timestamp;
        }

        virtual void OnDetach() {}

        Timestamp GetUpdateSpeed() {
            return m_UpdateDelta;
        }

        Timestamp GetLastUpdate() {
            return m_LastUpdate;
        }

        Timestamp GetUpdateDelta(const Anwill::Timestamp& timestamp) {
            return timestamp - m_LastUpdate;
        }

    protected:
        Timestamp m_UpdateDelta, m_LastUpdate;
    };
}