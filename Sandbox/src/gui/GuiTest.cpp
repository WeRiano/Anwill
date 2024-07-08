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
    Anwill::Gui::Text("This is some demo text.");
    Anwill::Gui::Text("This is some additional demo text on the same row.", false);

    // --- Button ---
    auto b1 = Anwill::Gui::Button("This is a button. Click me!", [](){});
    unsigned int* b1Count = new unsigned int;
    *b1Count = 0;
    b1->SetCallback([b1, b1Count](){
        (*b1Count)++;
        b1->SetText("You have clicked the button " + std::to_string(*b1Count) + " times!");
    });

    // --- Checkbox ---
    Anwill::Gui::Checkbox(true, "<-- This is a checkbox. Click me!", [](bool){});
    auto cb2 = Anwill::Gui::Checkbox(true,
                                     "<-- This checkbox has some styling customization.",
                                     [](bool){});
    cb2->m_Style->buttonShape = Anwill::GuiStyling::Button::Shape::Ellipse;
    cb2->m_Style->checkmarkType = Anwill::GuiStyling::Checkbox::CheckmarkType::Ellipse;
    cb2->m_Style->buttonColor = {0.8, 0.175, 0.745};
    cb2->m_Style->buttonHoverColor = {0.9, 0.3, 0.845};
    cb2->m_Style->buttonPressColor = {0.95, 0.35, 0.9};
    cb2->m_Style->checkmarkColor = {0.25, 1.0, 0.0};


    // --- Slider ---
    float* s1Val = new float;
    *s1Val = 10.0f;
    Anwill::Gui::Slider(0.0f, 42.0f, *s1Val, true, edd);
    int* s2Val = new int;
    *s2Val = 50;
    Anwill::Gui::Slider(-100, 100, *s2Val, true, edd);

    // --- Radio button ---
    int* e = new int;
    *e = 0;
    auto r1t = Anwill::Gui::Text("Radio button value is: " + std::to_string(*e));

    std::function<void()> r1f = [r1t, e](){
        r1t->SetText("Radio button value is: " + std::to_string(*e));
    };
    Anwill::Gui::RadioButton("Radio 1", *e, 1, r1f);
    Anwill::Gui::RadioButton("Radio 2", *e, 2, r1f, false);
    Anwill::Gui::RadioButton("Radio 3", *e, 3, r1f, false);

    // --- Input fields ---
    Anwill::Gui::TextInput("Select this field and type something.", 200);

    // --- Image ---
    Anwill::Gui::Image("Sandbox/assets/textures/anwill.jpg", 0);

    // --- Extra debug stuff --- // TODO: Remove
    Anwill::Gui::Button("Test button that is somewhat wide", [](){}, true);
}
