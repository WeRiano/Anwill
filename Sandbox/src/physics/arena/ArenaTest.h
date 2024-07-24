#pragma once

#include "Anwill.h"

class ArenaTest : public Anwill::Layer
{
public:
    static bool s_PlayerIsRound;
    static Anwill::EntityID s_Player;

    ArenaTest(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;
    void ImguiUpdate() override;

private:
    const Anwill::WindowSettings& m_WS;
    float m_VelocityMagnitude;
    float m_N;
    float m_CoR; // Coefficient of restitution
    float m_TimeMultiplier;
    unsigned int m_NumCollisions;
    bool m_WrapBodies;
    std::array<Anwill::EntityID, 4> m_Walls;

    void MoveAndRotatePlayer();
    void HandleCollisions();
    void WrapBodies(const Anwill::Timestamp delta);
    void TickBodies(Anwill::Timestamp delta);
    void SpawnWalls();
    void KillWalls();
};
