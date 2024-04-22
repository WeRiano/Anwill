#include "GuiTest.h"

GuiTest::GuiTest(const unsigned int ups)
        : Layer(ups), m_Counter(0), m_FloatSliderValue(0.0f)
{
    Anwill::Renderer::SetClearColor({0.3f, 0.4f, 0.5f});

    auto window1 = Anwill::Gui::CreateWindow("Test");
    Anwill::Gui::Text("Hey! This is some text.");
    Anwill::Gui::Text("Bonjour! This is some more text.", false);
    auto button = Anwill::Gui::Button("This is a button. Click it!");
    auto text = Anwill::Gui::Text("You have clicked the button " + std::to_string(m_Counter) + " times.",
                                  false);
    Anwill::Gui::Button("This is a useless button.");

    auto dd1 = Anwill::Gui::Dropdown("This is an awesome dropdown!");
    auto dd1Button = Anwill::Gui::Button("This is a button. Click it!", dd1);
    auto dd1Text = Anwill::Gui::Text("You have clicked the button " + std::to_string(m_Counter) + " times.",
                                     dd1,
                                     false);
    Anwill::Gui::Text("Dropdown element 3", dd1, true);
    Anwill::Gui::Button("Dropdown element 4", dd1, [](){}, false);
    Anwill::Gui::Text("Dropdown element 5", dd1, false);
    Anwill::Gui::Button("This is another button that does nothing!", dd1);
    Anwill::Gui::Checkbox(true, "Checkbox", dd1, [](bool b){});
    float floatSliderValue;
    Anwill::Gui::Slider(0.0f, 1337.42f, floatSliderValue, dd1);

    auto uselessButton = Anwill::Gui::Button("This is another button that does nothing!");
    Anwill::Gui::Checkbox(true, "Checkbox", [](bool b){});
    Anwill::Gui::Button("Test", [](){}, false);
    Anwill::Gui::Slider(0.0f, 420.69f, m_FloatSliderValue);
    int* intSliderValue = new int;
    *intSliderValue = 5;
    Anwill::Gui::Slider(1, 5, *intSliderValue);
    m_SliderTestText = Anwill::Gui::Text("Oh wow, the FloatSlider has a value of " + std::to_string(m_FloatSliderValue));

    button->SetCallback([this, text, dd1Text](){
        m_Counter++;
        text->Set("You have clicked the button " + std::to_string(m_Counter) + " times.");
        dd1Text->Set("You have clicked the button " + std::to_string(m_Counter) + " times.");
    });

    dd1Button->SetCallback([this, text, dd1Text](){
        m_Counter++;
        text->Set("You have clicked the button " + std::to_string(m_Counter) + " times.");
        dd1Text->Set("You have clicked the button " + std::to_string(m_Counter) + " times.");
    });

    uselessButton->EmplaceTooltip("Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Ut erat.\n"
                                  "Cum sociis natoque penatibus et magnis dis parturient montes, nascetur ridiculus mus.\n"
                                  "Mauris ligula. Suspendisse potenti.\nInteger odio ipsum, sagittis nec, ultrices quis, adipiscing eget, diam. "
                                  "Donec laoreet.\nEtiam sem velit, blandit sit amet, mollis in, pretium a, odio. In eu tellus a mi facilisis scelerisque. "
                                  "Donec bibendum pretium est.\nFusce ultricies eleifend tortor. "
                                  "Suspendisse mi tellus, condimentum vel, lobortis id, tempor a, quam.\n"
                                  "In vulputate, velit a porta lobortis, enim orci luctus enim, at iaculis turpis purus sit amet metus.\n"
                                  "Sed ac purus. Aliquam hendrerit.\nVestibulum eget augue sit amet wisi tincidunt cursus. "
                                  "In in orci. Phasellus eget ipsum.\nProin non magna nec erat vestibulum congue. Etiam pellentesque. "
                                  "Praesent sagittis porta ante.\nVestibulum nonummy viverra augue. In hac habitasse platea dictumst.\n"
                                  "Nulla a ipsum. Phasellus nulla velit, dignissim a, dignissim ut, sagittis et, mi.");

    int* e = new int;
    *e = 0;
    //int e = 0;
    Anwill::Gui::RadioButton("Radio 1", *e, 1);
    Anwill::Gui::RadioButton("Radio 2", *e, 2, [](){}, false);
    Anwill::Gui::RadioButton("Radio 3", *e, 3, [](){}, false);

    Anwill::Gui::TextInput("", 150.0f);

    auto id2 = Anwill::Gui::CreateWindow("Reallylongtitlethatprobablyshouldnotfittttttttttttttttttttttttttttttttttttttttt");
    Anwill::Gui::Text("Some text inside window! Cool!", true, id2);
    Anwill::Gui::Image("Sandbox/assets/textures/anwill.jpg", 2, id2);
    Anwill::Gui::Text("Some text after image", true, id2);
}

void GuiTest::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);
    m_SliderTestText->Set("Oh wow, the FloatSlider has a value of " + std::to_string(m_FloatSliderValue));
}