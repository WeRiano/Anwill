#pragma once

#include "Anwill.h"
#include "SandboxEvents.h"

class SandboxEventHandler
{
public:
    static void Init();

    template<typename E>
    static void Add(E const& child)
    {
        s_SysEvents.Add<E>(child);
    }

    static void Pop();

    template<class E>
    static void Subscribe(std::function<void(std::unique_ptr<Anwill::Event>&)> func)
    {
        s_SysEvents.Subscribe<E>(func);
    }

private:
    static Anwill::EventQueue s_SysEvents;
};