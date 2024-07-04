#include "GuiImage.h"
#include "utils/Profiler.h"
#include "math/Algo.h"
#include "gfx/Renderer2D.h"

namespace Anwill {

    GuiImage::GuiImage(const std::shared_ptr<GuiStyling::Container>& containerStyle, const std::string& fileName,
                       unsigned int maxRows, const std::shared_ptr<GuiStyling::Image>& style)
        : GuiElement(containerStyle),
          m_Style(style == nullptr ? std::make_shared<GuiStyling::Image>() : style),
          m_Texture(Texture::Create(fileName))
    {
        if(m_Texture->GetHeight() > maxRows * containerStyle->GetRowHeight()) {
            m_ScaleFactor = (maxRows) * containerStyle->GetRowHeight() / m_Texture->GetHeight();
        } else {
            m_ScaleFactor = 1.0f;
        }
        m_GridDepth = std::ceil(m_Texture->GetHeight() * m_ScaleFactor / containerStyle->GetRowHeight());
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