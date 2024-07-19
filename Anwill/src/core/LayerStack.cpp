#include "core/LayerStack.h"
#include "utils/Profiler.h"

namespace Anwill
{
    void LayerStack::Update()
    {
        AW_PROFILE_FUNC();

        for (int i = 0; i < m_Stack.size(); i++)
        {
            Timestamp last = m_Stack[i]->GetLastUpdate();
            Timestamp now = Timestamp();
            Timestamp delta = m_Stack[i]->GetUpdateSpeed();
            if (now - last >= delta || delta.IsZero()) {
                m_Stack[i]->Update(now);
            }
        }
    }

    void LayerStack::ImguiUpdate()
    {
        AW_PROFILE_FUNC();

        for (int i = 0; i < m_Stack.size(); i++) {
            m_Stack[i]->ImguiUpdate();
        }
    }
}