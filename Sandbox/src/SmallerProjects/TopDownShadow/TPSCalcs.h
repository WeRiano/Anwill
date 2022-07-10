#pragma once

#include "Anwill.h"

struct EntityComponent {
    Anwill::Math::Vec2f playerPos, dir;
    Anwill::Math::Vec3f camPos;
};

class TPSCalcs : public Anwill::Layer
{
public:
    TPSCalcs(const unsigned int ups);

    void Update(const Anwill::Timestamp &timestamp) override;

private:
    void OnEvent(std::unique_ptr<Anwill::Event>& e);
    void ChangePlayerLookDir(Anwill::MouseMoveEvent& e);
};
