#include "EditorLayer.h"

EditorLayer::EditorLayer(unsigned int ups, const Anwill::WindowSettings& ws)
    : Layer(ups)
{
    Anwill::Renderer::SetClearColor({0.3f, 0.3f, 0.3f});
    auto editorWindow = Anwill::Gui::CreateWindow("Editor");

}

void EditorLayer::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);
}
