#pragma once

#include <typeindex>

// Components are bigger in memory so they will be the limit
#define AW_MAX_COMPONENTS 200000
#define AW_MAX_ENTITIES AW_MAX_COMPONENTS

namespace Anwill {
    typedef long EntityID;

    // This is a type id. We don't need complete unique IDs for components(?), since they are linked to a entity
    // which has some unique ID
    typedef std::type_index ComponentID;
}