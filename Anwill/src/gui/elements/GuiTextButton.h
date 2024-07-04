#pragma once

#include <memory>

#include "GuiElement.h"
#include "GuiText.h"
#include "GuiButton.h"
#include "gui/styles/GuiStyling.h"
#include "core/Timestamp.h"

namespace Anwill {

    class GuiTextButton : public GuiText, public GuiButton {
    public:
        std::shared_ptr<GuiStyling::TextButton> m_Style;

        GuiTextButton(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                      const std::string& text,
                      unsigned int textSize,
                      const std::function<void()>& callback,
                      const std::shared_ptr<GuiStyling::TextButton>& style = nullptr);
        GuiTextButton(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                      const std::string& text,
                      unsigned int textSize,
                      unsigned int pixelWidth,
                      const std::function<void()>& callback,
                      const std::shared_ptr<GuiStyling::TextButton>& style = nullptr);

        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;
        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;

        void SetText(const std::string& text);

    protected:
        //GuiText m_Text;
        //GuiButton m_Button;
    };
}