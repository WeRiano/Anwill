#pragma once

#include <typeindex>
#include <bitset>

#define AW_MAX_ENTITIES 10000              // Maximum number of (unique) entities
#define AW_MAX_COMPONENTS AW_MAX_ENTITIES  // Maximum number of component instances for one type of component
#define AW_MAX_TYPE_COMPONENTS 100         // Maximum number of different types of components

namespace Anwill {
    // A signature which identifies what components an entity has.
    typedef std::bitset<AW_MAX_TYPE_COMPONENTS> CompSig;

    // Unique entity id. A long on the interval [0, AW_MAX_ENTITIES).
    typedef unsigned long EntityID;

    // This is a unique type/family id. An int on the interval [1, AW_MAX_TYPE_COMPONENTS].
    typedef unsigned int ComponentID;
}