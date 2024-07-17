#pragma once

#include "Anwill.h"

class GuiTest : public Anwill::Layer
{
public:
    GuiTest(const unsigned int ups);

    virtual void Update(const Anwill::Timestamp &timestamp) override;

private:
    unsigned int m_Counter;
    float m_FloatSliderValue;
    std::shared_ptr<Anwill::GuiText> m_SliderTestText;

    static void InitDemoWindow();
};
