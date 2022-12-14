#include "GuiTest.h"

GuiTest::GuiTest(const unsigned int ups)
        : Layer(ups), m_Counter(0)
{
    Anwill::Renderer::SetClearColor({0.3f, 0.4f, 0.5f});
    auto id1 = Anwill::Gui::CreateWindow("Test");
    Anwill::Gui::Text("Hey! This is some text.");
    auto button = Anwill::Gui::Button("This is a button. Click it!");
    auto text = Anwill::Gui::Text("You have clicked the button " + std::to_string(m_Counter) + " times.");
    Anwill::Gui::Text("More text to test the y thing ...");
    Anwill::Gui::Text("More text to test the y thing ...");
    Anwill::Gui::Button("This is a button that does nothing!");

    button->SetCallback([this, text](){
        m_Counter++;
        text->SetText("You have clicked the button " + std::to_string(m_Counter) + " times.");
    });

    /*
    auto id2 = Anwill::Gui::NewWindow("Reallylongtitlethatprobablyshouldnotfittttttttttttttttttttttttttttttttttttttttt");
    Anwill::Gui::Text("Some text inside window! Cool!", true, id2);

    Anwill::Gui::Text("More stupid text ... which is also super cool hehe xd", true, id2);
    Anwill::Gui::Text("AWESOME TEXT IN CAPSLOCK! AND SOME NUMBERS! 123456", true, id2);
    Anwill::Gui::Text("More stupid text ... which is also super cool hehe xd", true, id2);
     */
}

void GuiTest::Update(const Anwill::Timestamp& timestamp)
{

    Layer::Update(timestamp);
}