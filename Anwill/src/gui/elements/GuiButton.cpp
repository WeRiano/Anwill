#include "GuiButton.h"
#include "math/Algo.h"
#include "utils/Profiler.h"
#include "gfx/Renderer2D.h"

namespace Anwill {

    GuiButton::GuiButton(const std::shared_ptr<GuiStyling::Container>& containerStyle,
                         const Math::Vec2f& size, const std::function<void()>& callback,
                         const std::shared_ptr<GuiStyling::Button>& style)
        : GuiElement(containerStyle),
          m_Style(style == nullptr ? std::make_shared<GuiStyling::Button>() : style),
          m_ButtonSize(size),
          m_Callback(callback)
    {}

    void GuiButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                           const Timestamp& delta)
    {
        AW_PROFILE_FUNC();
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), m_ButtonSize);
        thisTransform = Math::Mat4f::Translate(thisTransform,
                                               assignedPos + Math::Vec2f(m_ButtonSize.X / 2.0f,
                                                                         -m_ButtonSize.Y / 2.0f));

        // Get shape of button from possible selections
        auto shader = GuiStyling::Button::shaders[(std::size_t) m_Style->buttonShape];
        shader->Bind();
        shader->SetUniform1i(m_IsHovered, "u_Hovering");
        shader->SetUniform1i(m_IsPressed, "u_Pressing");
        if(m_IsHovered)
        {
            shader->SetUniformVec3f(m_Style->buttonHoverColor, "u_HoverColor");
        }
        if(m_IsPressed)
        {
            shader->SetUniformVec3f(m_Style->buttonPressColor, "u_PressColor");
        }
        if(!(m_IsHovered || m_IsPressed))
        {
            shader->SetUniformVec3f(m_Style->buttonColor, "u_Color");
        }
        shader->Unbind();
        Renderer2D::SubmitMesh(shader, GuiStyling::rectMesh, thisTransform);
    }

    bool GuiButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        return Math::Algo::IsPointInsideRectangle({0.0f,        0.0f},
                                                  {m_ButtonSize.X, 0.0f},
                                                  {m_ButtonSize.X, -m_ButtonSize.Y},
                                                  {0.0f,        -m_ButtonSize.Y},
                                                  mousePos);
    }

    float GuiButton::GetWidth() const
    {
        return m_ButtonSize.X;
    }

    float GuiButton::GetHeight() const
    {
        return m_ButtonSize.Y;
    }

    Math::Vec2f GuiButton::GetSize() const
    {
        return m_ButtonSize;
    }

    unsigned int GuiButton::GetGridDepth() const
    {
        return 1;
    }

    void GuiButton::Release()
    {
        GuiElement::Release();
        m_Callback();
    }

    void GuiButton::SetCallback(const std::function<void()>& callback) {
        m_Callback = callback;
    }

    void GuiButton::SetWidth(float width)
    {
        m_ButtonSize = {width, m_ButtonSize.Y};
    }

    void GuiButton::SetHeight(float height)
    {
        m_ButtonSize = {m_ButtonSize.X, height};
    }
}