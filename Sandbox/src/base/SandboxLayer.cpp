#include "SandboxLayer.h"
#include "SandboxEventHandler.h"
#include "core/AppStats.h"
#include "ecs/Ecs.h"

SandboxLayer::SandboxLayer(unsigned int ups, const Anwill::WindowSettings& ws)
        : Layer(ups), m_IsRunningTestEnv(false)
{}

void SandboxLayer::Update(const Anwill::Timestamp& timestamp)
{
    if (ImGui::Begin("Sandbox test environments"))
    {
        if (m_IsRunningTestEnv)
        {
            ImGui::Text("GUI renders at %.2f fps (%.2f MS per frame)",
                        ImGui::GetIO().Framerate,
                        1000.0f / ImGui::GetIO().Framerate);
            ImGui::Text("Application updates %.2Lf times per second (%.2Lf MS per cycle)",
                        Anwill::AppStats::GetAppUpdatesPerSecond(), Anwill::AppStats::GetAppUpdateTimeMS());

            if(ImGui::Button("Return"))
            {
                SandboxEnvironmentEvent event(SandboxEnvironmentEvent::Env::None);
                SandboxEventHandler::Add(event);
                m_IsRunningTestEnv = false;
            }
        } else
        {
            if (ImGui::CollapsingHeader("Renderer"))
            {
                if (ImGui::Button("Primitive"))
                {
                    StartTestEnv(SandboxEnvironmentEvent::Env::Primitives);
                }
                if (ImGui::Button("Batch renderer"))
                {
                    StartTestEnv(SandboxEnvironmentEvent::Env::BatchRenderer);
                }
                if (ImGui::Button("Texture"))
                {
                    StartTestEnv(SandboxEnvironmentEvent::Env::Texture);
                }
                if (ImGui::Button("Font"))
                {
                    StartTestEnv(SandboxEnvironmentEvent::Env::Font);
                }
                if (ImGui::Button("Sprite animation"))
                {
                    StartTestEnv(SandboxEnvironmentEvent::Env::SpriteAnimation);
                }
            }
            if (ImGui::CollapsingHeader("Physics"))
            {
                if (ImGui::Button("Sprinkler"))
                {
                    StartTestEnv(SandboxEnvironmentEvent::Env::PhysicsCollision);
                }
                if (ImGui::Button("Arena"))
                {
                    StartTestEnv(SandboxEnvironmentEvent::Env::PhysicsDynamics);
                }
            }
        }
        ImGui::End();
    }

    SandboxEventHandler::Pop();

    Layer::Update(timestamp);
}

void SandboxLayer::StartTestEnv(SandboxEnvironmentEvent::Env env)
{
    SandboxEnvironmentEvent event(env);
    SandboxEventHandler::Add(event);
    m_IsRunningTestEnv = true;
}
