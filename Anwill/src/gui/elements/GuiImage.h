#pragma once

#include <memory>

#include "GuiElement.h"
#include "GuiText.h"
#include "GuiButton.h"
#include "gui/styles/GuiStyling.h"
#include "gfx/Texture.h"
#include "math/Vec2f.h"
#include "core/Timestamp.h"

namespace Anwill {

    class GuiImage : public GuiElement {
    public:
        std::shared_ptr<GuiStyling::Image> m_Style;

        GuiImage(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& fileName,
                 unsigned int maxRows, const std::shared_ptr<GuiStyling::Image>& style = nullptr);

        void Render(const Math::Vec2f& assignedPos,
                    const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override;

        bool IsHovering(const Math::Vec2f& mousePos) const override;
        float GetWidth() const override;
        unsigned int GetGridDepth() const override;

    private:
        std::shared_ptr<Texture> m_Texture;
        float m_ScaleFactor;
        unsigned int m_GridDepth;
    };
}
