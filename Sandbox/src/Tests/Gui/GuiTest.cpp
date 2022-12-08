#include "GuiTest.h"

GuiTest::GuiTest(const unsigned int ups)
        : Layer(ups)
{
    Anwill::Renderer::SetClearColor({0.3f, 0.4f, 0.5f});
    Anwill::Gui::NewWindow("Test");
    Anwill::Gui::NewWindow("Reallylongtitlethatprobablyshouldnotfittttttttttttttttttttttttttttttttttttttttt");
}

void GuiTest::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);
}