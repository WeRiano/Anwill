#include "GuiImage.h"
#include "utils/Profiler.h"
#include "math/Algo.h"
#include "gfx/Renderer2D.h"

namespace Anwill {

    GuiImage::GuiImage(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& fileName,
                       unsigned int maxRows, const std::shared_ptr<GuiStyling::Image>& style)
        : GuiElement(containerStyle),
          m_Style(AW_GUI_ENSURE_STYLE(style, GuiStyling::Image)),
          m_Texture(Texture::Create(fileName))
    {
        float totVerticalSpace = maxRows * containerStyle->GetRowHeight() - containerStyle->elementVerticalMargin;
        if(maxRows > 0 && m_Texture->GetHeight() > totVerticalSpace)
        {
            // Image is constrained to a specific number of rows set by user.
            m_GridDepth = maxRows;
            m_ScaleFactor = totVerticalSpace / m_Texture->GetHeight();
        } else if(m_Texture->GetHeight() < containerStyle->elementHeight) {
            // Image size is unconstrained and fits on a singular row.
            m_GridDepth = 1;
            m_ScaleFactor = containerStyle->elementHeight / m_Texture->GetHeight();
        } else {
            // Image is unconstrained and fits on n>1 rows. Find n.
            int imageHeightMinusFirstRow = m_Texture->GetHeight() - containerStyle->elementHeight;
            float rows = 1.0f + (imageHeightMinusFirstRow / containerStyle->GetRowHeight());
            m_GridDepth = std::floor( rows);
            totVerticalSpace = m_GridDepth * containerStyle->GetRowHeight() - containerStyle->elementVerticalMargin;
            m_ScaleFactor = totVerticalSpace / m_Texture->GetHeight();
        }
    }

    void GuiImage::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                          const Timestamp& delta)
    {
        AW_PROFILE_FUNC();

        Math::Vec2f imageSize = {static_cast<float>(m_Texture->GetWidth()),
                                 static_cast<float>(m_Texture->GetHeight())};
        imageSize = imageSize * m_ScaleFactor;
        Math::Mat4f transform = Math::Mat4f::Scale({}, imageSize);
        transform = Math::Mat4f::Translate(transform, {assignedPos.X + imageSize.X * 0.5f,
                                                       assignedPos.Y - imageSize.Y * 0.5f,
                                                       0.0f});
        Renderer2D::SubmitMesh(GuiStyling::Image::shader, Mesh::GetUnitRectangle(true),
                               transform, m_Texture);
    }

    bool GuiImage::IsHovering(const Math::Vec2f& mousePos) const {
        return Math::Algo::IsPointInsideRectangle({0.0f, 0.0f},
                                                  {static_cast<float>(m_Texture->GetWidth() * m_ScaleFactor), 0.0f},
                                                  {static_cast<float>(m_Texture->GetWidth() * m_ScaleFactor),
                                                   -static_cast<float>(m_Texture->GetHeight() * m_ScaleFactor)},
                                                  {0.0f, -static_cast<float>(m_Texture->GetHeight())},
                                                  mousePos);
    }

    float GuiImage::GetWidth() const {
        return m_Texture->GetWidth() * m_ScaleFactor;
    }

    unsigned int GuiImage::GetGridDepth() const {
        return m_GridDepth;
    }
}