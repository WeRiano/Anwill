#include "Layer2D.h"

Layer2D::Layer2D(const unsigned int ups)
    : Layer(ups)
{
}

void Layer2D::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);

    AW_INFO("{0}", timestamp.GetSeconds());
}
