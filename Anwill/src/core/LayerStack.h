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
         * Update each layer in the stack
         */
        void Update();

    private:
        std::vector<std::unique_ptr<Layer>> m_Stack;
    };
}