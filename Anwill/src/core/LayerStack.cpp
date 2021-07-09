#include "core/LayerStack.h"
#include "core/Log.h"

namespace Anwill
{
    void LayerStack::Update()
    {
        for(unsigned int i = 0; i < m_Stack.size(); i++)
        {
            Timestamp last = m_Stack[i]->GetLastUpdate();
            Timestamp now = Timestamp();
            if (now - last >= m_Stack[i]->GetUpdateDelta()) {
                m_Stack[i]->Update(now);
            }
        }
    }
}