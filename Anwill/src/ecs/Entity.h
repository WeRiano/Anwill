#pragma once

#include <typeinfo>
#include <unordered_map>

#include "EcsDef.h"

namespace Anwill {

    // Needs two types of ID:
    // 1. Completely unique ID to entity instance
    // 2. An ID that is shared between all entities of type T

    class Entity
    {
    public:
        Entity(bool alive=false);

        bool IsAlive();
        void Kill();
    private:
        //EntityID m_ID;
        bool m_Alive;
    };
}