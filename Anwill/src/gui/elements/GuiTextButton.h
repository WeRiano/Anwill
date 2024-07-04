#pragma once

#include <memory>

#include "GuiElement.h"
#include "GuiText.h"
#include "GuiButton.h"
#include "gui/styles/GuiStyling.h"
#include "core/Timestamp.h"

namespace Anwill {

    /**
     * A button which contains text.
     */
    class GuiTextButton : public GuiText, public GuiButton {
    public:
        std::shared_ptr<GuiStyling::TextButton> m_Style;

        // TODO: Client should be able to make a button without text and perhaps choose its size.
        /**
         * Create a text button with a dynamic width that adjusts to fit the width of its text content.
         */
        GuiTextButton(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                      const std::string& text,
                      const std::function<void()>& callback,
                      const std::shared_ptr<GuiStyling::TextButton>& style = nullptr);

        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;
        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;

        void SetText(const std::string& text);
    };
}