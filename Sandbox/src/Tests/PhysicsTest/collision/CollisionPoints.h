#pragma once

#include "Anwill.h"


class CollisionPoints : public Anwill::Layer
{
public:
    CollisionPoints(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;

private:
    bool m_IsRound;
    Anwill::EntityID m_Player;
    Anwill::OrthographicCamera m_Camera;
    std::shared_ptr<Anwill::Shader> m_RectShader, m_CircleShader;
    Anwill::Mesh m_Mesh;

    void MoveAndTiltPlayer(Anwill::RBody& pBody, Anwill::Math::Mat4f& transform);
    void HandleCollisions();
    void SwapShape();
};
