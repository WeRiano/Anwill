#pragma once

#include "Anwill.h"

class EditorLayer : public Anwill::Layer
{
public:
    EditorLayer(unsigned int ups, const Anwill::WindowSettings& ws);

    void Update(const Anwill::Timestamp& timestamp) override;
};
