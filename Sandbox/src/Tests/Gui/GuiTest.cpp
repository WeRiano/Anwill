#include "GuiTest.h"

GuiTest::GuiTest(const unsigned int ups)
        : Layer(ups), m_Counter(0)
{
    Anwill::Renderer::SetClearColor({0.3f, 0.4f, 0.5f});

    auto window1 = Anwill::Gui::CreateWindow("Test");
    Anwill::Gui::Text("Hey! This is some text.");
    Anwill::Gui::Text("Bonjour! This is some more text.", false);
    auto button = Anwill::Gui::Button("This is a button. Click it!");
    auto text = Anwill::Gui::Text("You have clicked the button " + std::to_string(m_Counter) + " times.", false);
    Anwill::Gui::Button("This is a useless button.");

    auto dd1 = Anwill::Gui::Dropdown("This is an awesome dropdown!");
    auto dd1Button = Anwill::Gui::Button("This is a button. Click it!", dd1);
    auto dd1Text = Anwill::Gui::Text("You have clicked the button " + std::to_string(m_Counter) + " times.", dd1, false);
    Anwill::Gui::Text("Dropdown element 3", dd1, true);
    Anwill::Gui::Button("Dropdown element 4", dd1, [](){}, false);
    Anwill::Gui::Text("Dropdown element 5", dd1, false);
    Anwill::Gui::Button("Dropdown element 6", dd1, [](){}, true);
    Anwill::Gui::Text("Dropdown element 7", dd1, true);
    Anwill::Gui::Button("Dropdown element 8", dd1, [](){}, true);

    Anwill::Gui::Button("This is another button that does nothing!");
    Anwill::Gui::Checkbox(true);

    button->SetCallback([this, text](){
        m_Counter++;
        text->SetText("You have clicked the button " + std::to_string(m_Counter) + " times.");
    });

    dd1Button->SetCallback([this, dd1Text](){
        m_Counter++;
        dd1Text->SetText("You have clicked the button " + std::to_string(m_Counter) + " times.");
    });

    auto id2 = Anwill::Gui::CreateWindow("Reallylongtitlethatprobablyshouldnotfittttttttttttttttttttttttttttttttttttttttt");
    Anwill::Gui::Text("Some text inside window! Cool!", true, id2);

    Anwill::Gui::Text("More stupid text ... which is also super cool hehe xd", true, id2);
    Anwill::Gui::Text("AWESOME TEXT IN CAPSLOCK! AND SOME NUMBERS! 123456", true, id2);
    Anwill::Gui::Text("More stupid text ... which is also super cool hehe xd", true, id2);
}

void GuiTest::Update(const Anwill::Timestamp& timestamp)
{

    Layer::Update(timestamp);
}