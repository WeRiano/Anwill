#include "SandboxEventHandler.h"

Anwill::EventQueue SandboxEventHandler::s_SysEvents = Anwill::EventQueue();

void SandboxEventHandler::Init()
{
    s_SysEvents.Register<SandboxEnvironmentEvent>();
}

void SandboxEventHandler::Pop()
{
    s_SysEvents.Pop();
}