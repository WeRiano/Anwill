#include "GuiTest.h"

GuiTest::GuiTest(const unsigned int ups)
        : Layer(ups), m_Counter(0)
{
    Anwill::Renderer::SetClearColor({0.3f, 0.4f, 0.5f});
    auto id1 = Anwill::Gui::NewWindow("Test");
    Anwill::Gui::Text("Hello! Bonjour! Holla! Hejsan! ", false, 0);
    Anwill::Gui::Text("World World! World! World! World!", false, 0);
    Anwill::Gui::Button("Hello! Bonjour! Holla! Hejsan!", [this]() {
        m_Counter++;
    }, true);
    Anwill::Gui::Text("Counter: " + std::to_string(m_Counter), true, 0);

    auto id2 = Anwill::Gui::NewWindow("Reallylongtitlethatprobablyshouldnotfittttttttttttttttttttttttttttttttttttttttt");
    Anwill::Gui::Text("Some text inside window! Cool!", true, id2);

    Anwill::Gui::Text("More stupid text ... which is also super cool hehe xd", true, id2);
    Anwill::Gui::Text("AWESOME TEXT IN CAPSLOCK! AND SOME NUMBERS! 123456", true, id2);
    Anwill::Gui::Text("More stupid text ... which is also super cool hehe xd", true, id2);
}

void GuiTest::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);
}