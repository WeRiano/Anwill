#include "TPSCalcs.h"

TPSCalcs::TPSCalcs(const unsigned int ups)
    : Anwill::Layer(ups)
{
    Anwill::Ecs::RegisterComponent<EntityComponent>();

    //Anwill::SystemEvents::Subscribe(AW_BIND_EVENT_FN(OnEvent), Anwill::EventType::KeyPress);
    //Anwill::SystemEvents::Subscribe(AW_BIND_EVENT_FN(OnEvent), Anwill::EventType::KeyRepeat);
    Anwill::SystemEvents::Subscribe(AW_BIND_EVENT_FN(OnEvent), Anwill::EventType::MouseMove);
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

void TPSCalcs::OnEvent(std::unique_ptr<Anwill::Event> &e)
{
    Anwill::EventHandler handler(*e);

    //handler.Handle<Anwill::KeyPressEvent>(AW_BIND_EVENT_FN(MovePlayer));
    //handler.Handle<Anwill::KeyRepeatEvent>(AW_BIND_EVENT_FN(MovePlayer));
    handler.Handle<Anwill::MouseMoveEvent>(AW_BIND_EVENT_FN(ChangePlayerLookDir));
}

void TPSCalcs::ChangePlayerLookDir(Anwill::MouseMoveEvent& e)
{
    Anwill::Ecs::ForEach<EntityComponent>([e](Anwill::EntityID id, EntityComponent& comp) {
        comp.dir = Anwill::Math::Vec2f(e.GetXPos() - (comp.playerPos.GetX() + comp.camPos.GetX()),
                                       e.GetYPos() - (comp.playerPos.GetY() + comp.camPos.GetY()));
        comp.dir.Normalize();
    });
}
