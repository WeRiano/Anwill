#pragma once

#include "core/Layer.h"
#include "core/Window.h"
#include "gui/Gui.h"

namespace Anwill {

    class GuiLayer : public Layer
    {
    public:
        GuiLayer(unsigned int ups, const WindowSettings& windowSettings);

        void Update(const Timestamp& timestamp) override;
    };
}
