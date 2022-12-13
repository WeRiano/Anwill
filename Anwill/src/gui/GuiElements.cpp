#include "gui/GuiElements.h"
#include "math/Algo.h"
#include "core/Log.h"

namespace Anwill {

    Mesh GuiElement::s_RectMesh;
    std::unique_ptr<Font> GuiElement::s_Font;
    std::shared_ptr<Shader> GuiText::s_Shader;
    std::shared_ptr<Shader> GuiButton::s_Shader;
    std::shared_ptr<Shader> GuiWindow::s_WindowShader;

    // ---------- ELEMENT ----------

    GuiElement::GuiElement(const Math::Vec2f& position)
        : m_Pos(position), m_IsHovered(false), m_IsPressed(false)
    {}

    void GuiElement::Move(const Math::Vec2f& delta)
    {
        m_Pos += delta;
    }

    void GuiElement::StopHovering()
    {
        m_IsHovered = false;
    }

    void GuiElement::StopPressing()
    {
        m_IsPressed = false;
    }

    Math::Vec2f GuiElement::GetPos() const
    {
        return m_Pos;
    }

    Math::Vec2f GuiElement::GetSize() const
    {
        return m_Size;
    }

    // ---------- TEXT ----------

    GuiText::GuiText(const std::string& text, unsigned int textSize, const Math::Vec2f& position)
        : GuiElement(position), m_Text(text), m_TextScale(Font::GetScaleValue(textSize)), m_TextWidth((float) GuiText::s_Font->GetStringWidth(text))
    {
        m_Size = { m_TextWidth * m_TextScale, GuiMetrics::WindowElementHeight };
        m_Pos += { 0.0f, -GuiMetrics::TextBaselineOffset };
    }

    void GuiText::Render(const Math::Vec2f& parentPos, const Math::Vec2f& parentSize)
    {
        float cutoffXPos = parentPos.GetX() + parentSize.GetX() - GuiMetrics::WindowCutoffMargin;
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), {m_TextScale, m_TextScale, 1.0f});
        thisTransform = Math::Mat4f::Translate(thisTransform, parentPos + m_Pos);

        // Render
        GuiText::s_Shader->Bind();
        GuiText::s_Shader->SetUniform1f(cutoffXPos, "u_CutoffWidth");
        Renderer2D::Submit(GuiText::s_Shader, *GuiElement::s_Font, m_Text, thisTransform);
    }

    bool GuiText::IsHovering(const Math::Vec2f& mousePos) { return false; }
    void GuiText::OnHover() { return; }
    void GuiText::OnPress() { return; }
    void GuiText::OnRelease() { return; }

    // ---------- BUTTON ----------

    GuiButton::GuiButton(const std::string& text, unsigned int textSize, const std::function<void()>& callback, const Math::Vec2f& position)
        : GuiElement(position), m_ButtonText(text, textSize, position), m_Callback(callback)
    {
        m_Size = { m_ButtonText.GetSize().GetX() + GuiMetrics::ButtonTextMargin * 2.0f, GuiMetrics::WindowElementHeight };
        m_Pos += { 0.0f, 0.0f };
        m_ButtonText.Move({ GuiMetrics::ButtonTextMargin, 0.0f});
    }

    void GuiButton::Render(const Math::Vec2f& parentPos, const Math::Vec2f& parentSize)
    {
        float cutoffXPos = parentPos.GetX() + parentSize.GetX() - GuiMetrics::WindowCutoffMargin;
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), m_Size);
        thisTransform = Math::Mat4f::Translate(thisTransform, parentPos + m_Pos + Math::Vec2f(m_Size.GetX() / 2.0f, -m_Size.GetY() / 2.0f));

        // Render button
        GuiButton::s_Shader->Bind();
        GuiButton::s_Shader->SetUniform1i(m_IsHovered, "u_Hovering");
        GuiButton::s_Shader->SetUniform1i(m_IsPressed, "u_Pressing");
        GuiButton::s_Shader->SetUniform1f(cutoffXPos, "u_CutoffWidth");
        Renderer2D::Submit(GuiButton::s_Shader, GuiElement::s_RectMesh, thisTransform);

        m_ButtonText.Render(parentPos, parentSize);
    }

    bool GuiButton::IsHovering(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  {m_Pos.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  mousePos);
    }

    void GuiButton::OnHover()
    {
        m_IsHovered = true;
    }

    void GuiButton::OnPress()
    {
        m_IsPressed = true;
    }

    void GuiButton::OnRelease()
    {
        m_IsPressed = false;
    }

    void GuiDropdown::Render(const Math::Vec2f& parentPos, const Math::Vec2f& parentSize)
    {

    }

    // -------- WINDOW --------

    GuiWindow::GuiWindow(const std::string& title, GuiWindowID id, const Math::Vec2f& position, const Math::Vec2f& size)
            : m_Pos(position), m_Size(size), m_ID(id),
              m_Title(title, 13, {GuiMetrics::WindowBorderSize * 2 + GuiMetrics::WindowElementMargin, GuiMetrics::TextBaselineOffset - GuiMetrics::WindowHeaderSize * 0.75f}),
              m_NextHorizontalElementPos(GuiMetrics::WindowElementMargin, -(GuiMetrics::WindowHeaderSize + GuiMetrics::WindowElementMargin)),
              m_NextVerticalElementPos(m_NextHorizontalElementPos)
    {}

    void GuiWindow::Render(bool selected)
    {
        s_WindowShader->Bind();
        s_WindowShader->SetUniform1i(selected, "u_Selected");
        s_WindowShader->Unbind();

        auto transform = Math::Mat4f::Translate(Math::Mat4f::Identity(), m_Pos + Math::Vec2f(m_Size.GetX() / 2.0f, -m_Size.GetY() / 2.0f));
        transform = Math::Mat4f::Scale(transform, m_Size);
        Renderer2D::Submit(s_WindowShader, GuiElement::s_RectMesh, transform);

        m_Title.Render(m_Pos, m_Size);

        for (unsigned int i = 0; i < m_Elements.size(); i++) {
            m_Elements[i]->Render(m_Pos, m_Size);
        }
    }

    bool GuiWindow::IsHoveringHeader(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - GuiMetrics::WindowHeaderSize},
                                                  {m_Pos.GetX(), m_Pos.GetY() - GuiMetrics::WindowHeaderSize},
                                                   mousePos);
    }

    bool GuiWindow::IsHoveringResize(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideCircle({m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                               10.0f, mousePos);
    }

    bool GuiWindow::IsHoveringWindow(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  {m_Pos.GetX(), m_Pos.GetY() - m_Size.GetY()},
                                                  mousePos);
    }

    std::shared_ptr<GuiElement> GuiWindow::GetHoverElement(const Math::Vec2f& mousePos)
    {
        for(unsigned int i = 0; i < m_Elements.size(); i++) {
            if(m_Elements[i]->IsHovering(mousePos - GetPos())) {
                return m_Elements[i];
            }
        }
        return nullptr;
    }

    void GuiWindow::Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos)
    {
        if(!(m_Pos.GetY() + delta.GetY() > maxPos.GetY() ||
             m_Pos.GetY() - GuiMetrics::WindowHeaderSize + delta.GetY() < minPos.GetY())) {
            m_Pos = {m_Pos.GetX(), m_Pos.GetY() + delta.GetY()};
        }
        if(!(m_Pos.GetX() + delta.GetX() < minPos.GetX() ||
             m_Pos.GetX() + m_Size.GetX() + delta.GetX() > maxPos.GetX())) {
            m_Pos = {m_Pos.GetX() + delta.GetX(), m_Pos.GetY()};
        }
    }

    void GuiWindow::Resize(const Math::Vec2f& delta, const Math::Vec2f& minSize, const Math::Vec2f& maxSize)
    {
        Math::Vec2f newSize = m_Size + delta;
        newSize.Clamp(minSize, maxSize);
        m_Size = newSize;
    }

    Math::Vec2f GuiWindow::GetPos()
    {
        return m_Pos;
    }

    GuiWindowID GuiWindow::GetID()
    {
        return m_ID;
    }
}