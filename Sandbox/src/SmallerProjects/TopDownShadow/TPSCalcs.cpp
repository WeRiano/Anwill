#include "TPSCalcs.h"

TPSCalcs::TPSCalcs(const unsigned int ups)
    : Anwill::Layer(ups)
{
    Anwill::Ecs::RegisterComponent<EntityComponent>();
    Anwill::SystemEvents::Subscribe<Anwill::MouseMoveEvent>(
            AW_BIND_THIS_MEMBER_FUNC(ChangePlayerLookDir));
}


void TPSCalcs::Update(const Anwill::Timestamp &timestamp)
{
    Layer::Update(timestamp);

    Anwill::Ecs::ForEach<EntityComponent>([](Anwill::EntityID id, EntityComponent& comp) {
        float speed = 5.0f;

        //AW_INFO("Press key: {0}", (unsigned int) e.GetKeyCode());
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::W)) {
            comp.playerPos.Move(0.0f, speed);
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::A)) {
            comp.playerPos.Move(-speed, 0.0f);
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::D)) {
            comp.playerPos.Move(speed, 0.0f);
        }
        if (Anwill::Input::IsKeyPressed(Anwill::KeyCode::S)) {
            comp.playerPos.Move(0.0f, -speed);
        }
    });
}

void TPSCalcs::ChangePlayerLookDir(std::unique_ptr<Anwill::Event>& event)
{
    auto e = static_cast<Anwill::MouseMoveEvent&>(*event);
    Anwill::Ecs::ForEach<EntityComponent>([e](Anwill::EntityID id, EntityComponent& comp) {
        comp.dir = Anwill::Math::Vec2f(e.GetXPos() - (comp.playerPos.GetX() + comp.camPos.GetX()),
                                       e.GetYPos() - (comp.playerPos.GetY() + comp.camPos.GetY()));
        comp.dir.Normalize();
    });
}
