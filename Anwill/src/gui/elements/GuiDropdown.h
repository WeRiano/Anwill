#pragma once

#include <memory>

#include "GuiElement.h"
#include "GuiTextButton.h"
#include "gui/containers/GuiContainer.h"
#include "gui/styles/GuiStyling.h"
#include "gfx/Texture.h"
#include "math/Vec2f.h"
#include "core/Timestamp.h"

namespace Anwill {

    class GuiDropdown : public GuiTextButton, public GuiContainer {
    public:
        std::shared_ptr<GuiStyling::Dropdown> m_Style;

        GuiDropdown(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& text,
                    const std::shared_ptr<GuiStyling::Dropdown>& style = nullptr);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        unsigned int GetGridDepth() const override;
    };
}
