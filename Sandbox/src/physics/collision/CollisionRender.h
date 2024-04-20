#pragma once

#include "Anwill.h"

class CollisionRender : public Anwill::Layer
{
public:
    CollisionRender(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    static bool s_PlayerIsRound;
    static Anwill::EntityID s_Player;
    static Anwill::Mesh s_Mesh;
    Anwill::OrthographicCamera m_Camera;
    std::shared_ptr<Anwill::Shader> m_RectShader, m_CircleShader;
};
