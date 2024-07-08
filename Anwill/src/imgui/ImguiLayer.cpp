#include "ImguiLayer.h"

namespace Anwill {

    void ImguiLayer::Init()
    {

    }

    ImguiLayer::ImguiLayer(const unsigned int ups)
        : Layer(ups)
    {
        Init();
    }

    void ImguiLayer::Update(const Timestamp& timestamp)
    {
        Layer::Update(timestamp);
    }
}