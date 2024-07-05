#include "GuiTest.h"

GuiTest::GuiTest(const unsigned int ups)
        : Layer(ups), m_Counter(0), m_FloatSliderValue(0.0f)
{
    Anwill::Renderer::SetClearColor({0.3f, 0.4f, 0.5f});

    InitDemoWindow();
}

void GuiTest::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);
}

void GuiTest::InitDemoWindow()
{
    auto demoWindow = Anwill::Gui::CreateWindow("Anwill Gui Demo");
    auto edd = Anwill::Gui::Dropdown("Elements");

    // --- Text ---
    Anwill::Gui::Text("This is some demo text.", edd);
    Anwill::Gui::Text("This is some additional demo text on the same row.", edd, false);

    // --- Button ---
    auto b1 = Anwill::Gui::Button("This is a button. Click me!", edd);
    unsigned int* b1Count = new unsigned int;
    *b1Count = 0;
    b1->SetCallback([b1, b1Count](){
        (*b1Count)++;
        b1->SetText("You have clicked the button " + std::to_string(*b1Count) + " times");
    });

    // --- Slider ---
    float* s1Val = new float;
    *s1Val = 10.0f;
    Anwill::Gui::Slider(0.0f, 42.0f, *s1Val, edd);
    int* s2Val = new int;
    *s2Val = 50;
    Anwill::Gui::Slider(-100, 100, *s2Val, edd);
}
