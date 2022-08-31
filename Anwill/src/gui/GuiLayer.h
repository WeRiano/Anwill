#pragma once

#include "core/Layer.h"

namespace Anwill {

    class GuiLayer : public Layer
    {
    public:
        GuiLayer(unsigned int ups);

        void Update(const Timestamp& timestamp) override;
    };
}
