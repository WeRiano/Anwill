#pragma once

#include <memory>

// Need to use this to pass a non-static member function as a parameter.
// Member functions need to be called on an object which is passed as an implicit
// argument to the function. If you want to bind a non-static member
// function, global function or lambda, don't use this. Just pass it like normal.
#define AW_BIND_THIS_MEMBER_FUNC(func) [this](auto&&... args) -> \
decltype(auto) { return this->func(std::forward<decltype(args)>(args)...); }

namespace Anwill {

    template<typename T>
    using Unique = std::unique_ptr<T>;
    template<typename T, typename... Args>
    constexpr Unique<T> CreateUnique(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Shared = std::shared_ptr<T>;
    template<typename T, typename... Args>
    constexpr Shared<T> CreateShared(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Temp = std::weak_ptr<T>;
}

