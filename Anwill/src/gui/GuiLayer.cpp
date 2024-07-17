#include "GuiLayer.h"

namespace Anwill {

    GuiLayer::GuiLayer(unsigned int ups, const WindowSettings& windowSettings)
        : Layer(ups)
    {
        Gui::Init(windowSettings);
    }

    void GuiLayer::Update(const Timestamp& timestamp)
    {
        Timestamp delta = timestamp - m_LastUpdate;
        Gui::Update();
        Gui::Render(delta);

        Layer::Update(timestamp);
    }
}