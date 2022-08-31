#include "gui/GuiLayer.h"

namespace Anwill {

    GuiLayer::GuiLayer(unsigned int ups)
       : Layer(ups)
    {}

    void GuiLayer::Update(const Timestamp& timestamp)
    {


        Layer::Update(timestamp);
    }
}