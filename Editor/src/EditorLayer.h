#pragma once

#include "Anwill.h"

namespace Anwill {

    class EditorLayer : public Layer
    {
    public:
        EditorLayer(unsigned int ups, const Anwill::WindowSettings& ws);

        void Update(const Anwill::Timestamp& timestamp) override;
    };
}