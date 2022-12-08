#include "GuiTest.h"

GuiTest::GuiTest(const unsigned int ups)
        : Layer(ups), m_Camera(1200.0f, 900.0f)
{
    Anwill::Renderer::SetClearColor({0.3f, 0.4f, 0.5f});
    Anwill::Gui::NewWindow("Test");
    Anwill::Gui::NewWindow("Reallylongtitlethatprobablyshouldnotfittttttttttttttttttttttttttttttttttttttttt");
}

void GuiTest::Update(const Anwill::Timestamp& timestamp)
{
    Anwill::Renderer2D::BeginScene(m_Camera);

    Layer::Update(timestamp);
}