#pragma once

#include <memory>

#include "GuiElement.h"
#include "GuiTextButton.h"
#include "gui/styles/GuiStyling.h"
#include "math/Vec2f.h"
#include "core/Timestamp.h"

namespace Anwill {

    class GuiCheckbox : public GuiButton, public GuiText  {
    public:
        std::shared_ptr<GuiStyling::Checkbox> m_Style;

        GuiCheckbox(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                    bool checked,
                    const std::string& text,
                    const std::function<void(bool)>& callback,
                    const std::shared_ptr<GuiStyling::Checkbox>& style = nullptr);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;

    protected:
        bool m_Checked;
    };
}
