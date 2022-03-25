#pragma once

#include <typeinfo>

#include "core/Log.h"

namespace Anwill {

    class Component
    {
    public:
        virtual void Identify() = 0;
    };
}