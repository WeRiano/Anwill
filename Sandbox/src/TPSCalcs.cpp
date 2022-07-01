#include "TPSCalcs.h"

TPSCalcs::TPSCalcs(const unsigned int ups)
    : Anwill::Layer(ups)
{
    Anwill::Ecs::Initialize();
    Anwill::Ecs::RegisterComponent<PlayerComponent>();

    Anwill::SystemEvents::Subscribe(AW_BIND_EVENT_FN(OnEvent), Anwill::EventType::KeyPress);
    Anwill::SystemEvents::Subscribe(AW_BIND_EVENT_FN(OnEvent), Anwill::EventType::KeyRepeat);
    Anwill::SystemEvents::Subscribe(AW_BIND_EVENT_FN(OnEvent), Anwill::EventType::MouseMove);
}


void TPSCalcs::Update(const Anwill::Timestamp &timestamp)
{
    Layer::Update(timestamp);
}

void TPSCalcs::OnEvent(std::unique_ptr<Anwill::Event> &e)
{
    Anwill::EventHandler handler(*e);
    handler.Handle<Anwill::KeyPressEvent>(AW_BIND_EVENT_FN(MovePlayer));
    handler.Handle<Anwill::KeyRepeatEvent>(AW_BIND_EVENT_FN(MovePlayer));
    handler.Handle<Anwill::MouseMoveEvent>(AW_BIND_EVENT_FN(ChangePlayerLookDir));
}

void TPSCalcs::MovePlayer(Anwill::KeyPressEvent& e)
{
    Anwill::Ecs::ForEach<PlayerComponent>([e](Anwill::EntityID id, PlayerComponent& comp) {
        float speed = 5.0f;

        if (e.GetKeyCode() == Anwill::KeyCode::W) {
            comp.playerPos.Move(0.0f, speed);
        } else if (e.GetKeyCode() == Anwill::KeyCode::A) {
            comp.playerPos.Move(-speed, 0.0f);
        } else if (e.GetKeyCode() == Anwill::KeyCode::D) {
            comp.playerPos.Move(speed, 0.0f);
        } else if (e.GetKeyCode() == Anwill::KeyCode::S) {
            comp.playerPos.Move(0.0f, -speed);
        }
    });
}

void TPSCalcs::MovePlayer(Anwill::KeyRepeatEvent& e)
{
    Anwill::Ecs::ForEach<PlayerComponent>([e](Anwill::EntityID id, PlayerComponent& comp) {
        float speed = 10.0f;

        if (e.GetKeyCode() == Anwill::KeyCode::W) {
            comp.playerPos.Move(0.0f, speed);
        }
        if (e.GetKeyCode() == Anwill::KeyCode::A) {
            comp.playerPos.Move(-speed, 0.0f);
        }
        if (e.GetKeyCode() == Anwill::KeyCode::D) {
            comp.playerPos.Move(speed, 0.0f);
        }
        if (e.GetKeyCode() == Anwill::KeyCode::S) {
            comp.playerPos.Move(0.0f, -speed);
        }
    });
}

void TPSCalcs::ChangePlayerLookDir(Anwill::MouseMoveEvent& e)
{
    Anwill::Ecs::ForEach<PlayerComponent>([e](Anwill::EntityID id, PlayerComponent& comp) {
        comp.dir = Anwill::Math::Vec2f(e.GetXPos() - (comp.playerPos.GetX() + comp.camPos.GetX()),
                                       e.GetYPos() - (comp.playerPos.GetY() + comp.camPos.GetY()));
        comp.dir.Normalize();
    });
}
