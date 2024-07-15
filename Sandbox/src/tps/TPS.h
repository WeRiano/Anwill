#pragma once

#include "Anwill.h"

class TPS : public Anwill::Layer {
public:
    TPS(const unsigned int ups, float chunkWidth, float chunkHeight, unsigned int worldWidth,
        unsigned int worldHeight, const Anwill::WindowSettings& ws);

    virtual void Update(const Anwill::Timestamp &timestamp) override;
private:

    void CreateWallSegments();

    // Program parameters and info
    Anwill::Math::Mat4f m_PlayerTransform;

    // Objects and stuff
    Anwill::OrthographicCamera m_Camera;
    Anwill::Mesh m_WorldMesh, m_PlayerMesh;
    std::shared_ptr<Anwill::Shader> m_ShadowShader, m_PlayerShader;
};
