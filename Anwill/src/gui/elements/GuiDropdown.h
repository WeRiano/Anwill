#pragma once

#include <memory>

#include "GuiElement.h"
#include "GuiText.h"
#include "GuiButton.h"
#include "gui/containers/GuiContainer.h"
#include "gui/styles/GuiStyling.h"
#include "gfx/Texture.h"
#include "math/Vec2f.h"
#include "core/Timestamp.h"

namespace Anwill {

    class GuiDropdown : public GuiElement {
    public:
        std::shared_ptr<GuiStyling::Dropdown> m_Style;

        GuiDropdown(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& text,
                    unsigned int textSize, const std::shared_ptr<GuiStyling::Dropdown>& style = nullptr);

        std::shared_ptr<GuiElement> GetHoverElement(Math::Vec2f& hoverElementPos, const Math::Vec2f& mousePos) const;
        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;

    protected:
        GuiContainer m_Container;
        GuiText m_Text;
        GuiButton m_Button;
    };
}
