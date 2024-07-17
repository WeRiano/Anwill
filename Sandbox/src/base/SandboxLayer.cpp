#include "SandboxLayer.h"
#include "SandboxEventHandler.h"
#include "core/AppStats.h"
#include "ecs/Ecs.h"

SandboxLayer::SandboxLayer(unsigned int ups, const Anwill::WindowSettings& ws)
        : Layer(ups)
{

}

void SandboxLayer::Update(const Anwill::Timestamp& timestamp)
{
    Layer::Update(timestamp);

    if(ImGui::Begin("Anwill sandbox selector")) {
        ImGui::Text("Hello world!");
    }
    ImGui::End();
}