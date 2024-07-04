#pragma once

#include <memory>

#include "GuiElement.h"
#include "GuiText.h"
#include "GuiButton.h"
#include "gui/styles/GuiStyling.h"
#include "math/Vec2f.h"
#include "core/Timestamp.h"

namespace Anwill {

    class GuiRadioButton : public GuiButton, public GuiText {
    public:
        std::shared_ptr<GuiStyling::RadioButton> m_Style;

        GuiRadioButton(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& text,
                       int& reference, int onSelectValue, const std::function<void()>& callback,
                       const std::shared_ptr<GuiStyling::RadioButton>& style = nullptr);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;

    protected:
        int& m_Reference;
        const int m_OnSelectValue;
    };
}