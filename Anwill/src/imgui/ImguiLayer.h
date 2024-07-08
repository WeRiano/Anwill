#pragma once

#include <imgui.h>

#include "core/Core.h"
#include "core/Layer.h"

namespace Anwill {

    class ImguiLayer : public Layer
    {
    public:
        static void Init();

        explicit ImguiLayer(unsigned int ups);

        void Update(const Timestamp& timestamp) override;
    };
}
