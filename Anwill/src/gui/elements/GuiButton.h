#pragma once

#include <memory>

#include "GuiElement.h"

namespace Anwill {

    class GuiButton : public virtual GuiElement {
    public:
        std::shared_ptr<GuiStyling::Button> m_Style;

        GuiButton(const std::shared_ptr<GuiStyling::Container>& containerStyle, const Math::Vec2f& size,
                  const std::function<void()>& callback, const std::shared_ptr<GuiStyling::Button>& style = nullptr);

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;
        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        float GetHeight() const;
        Math::Vec2f GetSize() const;
        unsigned int GetGridDepth() const override;
        void Release() override;

        void SetCallback(const std::function<void()>& callback);
        void SetWidth(float width);
        void SetHeight(float height);

    protected:
        Math::Vec2f m_ButtonSize;
        std::function<void()> m_Callback;
    };
}