#pragma once

#include "Anwill.h"

struct PlayerComponent {
    Anwill::Math::Vec2f playerPos, dir, camPos;
};

class TPSCalcs : public Anwill::Layer
{
public:
    TPSCalcs(const unsigned int ups);

    void Update(const Anwill::Timestamp &timestamp) override;

private:
    void OnEvent(std::unique_ptr<Anwill::Event>& e);
    void MovePlayer(Anwill::KeyPressEvent& e);
    void MovePlayer(Anwill::KeyRepeatEvent& e);
    void ChangePlayerLookDir(Anwill::MouseMoveEvent& e);
};
