#pragma once

#include <vector>
#include <memory>

#include "Layer.h"

namespace Anwill {

    class LayerStack
    {
    public:
        /**
         * Add a layer to the stack
         * @return
         */
        template <class C, typename... Args>
        void Add(const unsigned int ups, Args&&... args)
        {
            m_Stack.emplace(m_Stack.begin(),
                            std::move(std::make_unique<C>(ups, std::forward<Args>(args)...)));
        }

        /**
         * Remove first occurrence of layer (based on type).
         * @tparam C
         */
        template <class C>
        void Remove()
        {
            for(int i = 0; i < m_Stack.size(); i++)
            {
                if( dynamic_cast<C*>(m_Stack[i].get()) != nullptr )
                {
                    m_Stack.erase(m_Stack.begin() + i);
                    break;
                }
            }
        }

        /**
         * Update each layer in the stack.
         * @return Combined duration it took to update the entire stack.
         */
        Timestamp Update();

    private:
        std::vector<std::unique_ptr<Layer>> m_Stack;
    };
}