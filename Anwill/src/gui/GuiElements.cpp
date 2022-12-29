#include "gui/GuiElements.h"
#include "math/Algo.h"
#include "core/Log.h"

namespace Anwill {

    Math::Vec2f GuiMetrics::windowSize;
    Mesh GuiElement::s_RectMesh;
    Mesh GuiElement::s_TriangleMesh;
    std::shared_ptr<Shader> GuiElement::s_PrimitiveShader;
    std::unique_ptr<Font> GuiElement::s_Font;
    std::shared_ptr<Shader> GuiText::s_Shader;
    std::shared_ptr<Shader> GuiTextButton::s_Shader;

    // ---------- ELEMENT ----------

    GuiElement::GuiElement(bool onNewRow, bool forceNextToNewRow)
        : m_IsHovered(false), m_IsPressed(false),
          m_OnNewRow(onNewRow), m_ForceNextToNewRow(forceNextToNewRow)
    {}

    void GuiElement::StartHovering()
    {
        m_IsHovered = true;
    }

    void GuiElement::StopHovering()
    {
        m_IsHovered = false;
    }

    void GuiElement::StartPressing()
    {
        m_IsPressed = true;
    }

    void GuiElement::StopPressing()
    {
        m_IsPressed = false;
    }

    void GuiElement::Release()
    {
        m_IsPressed = false;
    }

    bool GuiElement::IsHovered() const
    {
        return m_IsHovered;
    }

    bool GuiElement::OnNewRow() const
    {
        return m_OnNewRow;
    }

    bool GuiElement::ForceNextToNewRow() const
    {
        return m_ForceNextToNewRow;
    }

    // ---------- TEXT ----------

    GuiText::GuiText(bool onNewRow, const std::string& text, unsigned int textSize)
        : GuiElement(onNewRow, false), m_TextPos(1.0f, -GuiMetrics::WindowElementHeight / 2.0f + GuiMetrics::TextBaselineOffset), m_Text(text),
          m_TextScale(Font::GetScaleValue(textSize)), m_TextWidth((float) GuiText::s_Font->GetStringWidth(text) * m_TextScale)
    {}

    void GuiText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), {m_TextScale, m_TextScale, 1.0f});
        thisTransform = Math::Mat4f::Translate(thisTransform, m_TextPos + assignedPos);

        // Render
        GuiText::s_Shader->Bind();
        GuiText::s_Shader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiText::s_Shader, *GuiElement::s_Font, m_Text, thisTransform);
    }

    bool GuiText::IsHovering(const Math::Vec2f& mousePos) const {
        return false;
    }

    float GuiText::GetWidth() const
    {
        return m_TextWidth;
    }

    unsigned int GuiText::GetGridDepth() const
    {
        return 1;
    }

    void GuiText::SetText(const std::string& text) {
        m_Text = text;
        m_TextWidth = (float) GuiText::s_Font->GetStringWidth(text) * m_TextScale;
    }

    // ---------- BUTTON ----------

    GuiTextButton::GuiTextButton(bool onNewRow, const std::string& text, unsigned int textSize, const std::function<void()>& callback)
        : GuiText(onNewRow, text, textSize),
          m_ButtonSize(GuiText::GetWidth() + GuiMetrics::ButtonTextMargin * 2.0f, GuiMetrics::WindowElementHeight),
          m_Callback(callback)
    {
        // Text pos adjusted slightly to the right such that it is positioned in the middle of the button
        m_TextPos += {GuiMetrics::ButtonTextMargin, 0.0f};
    }

    void GuiTextButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f cutoffPos = GuiMetrics::GetCutoffPos(assignedPos, assignedMaxSize);
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), m_ButtonSize);
        thisTransform = Math::Mat4f::Translate(thisTransform,
                                               assignedPos + Math::Vec2f(m_ButtonSize.GetX() / 2.0f, -m_ButtonSize.GetY() / 2.0f));

        // Render button
        GuiTextButton::s_Shader->Bind();
        GuiTextButton::s_Shader->SetUniform1i(m_IsHovered, "u_Hovering");
        GuiTextButton::s_Shader->SetUniform1i(m_IsPressed, "u_Pressing");
        GuiTextButton::s_Shader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiTextButton::s_Shader, GuiElement::s_RectMesh, thisTransform);

        // Render text
        GuiText::Render(assignedPos, assignedMaxSize);
    }

    bool GuiTextButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        return Math::Algo::IsPointInsideRectangle({0.0f,                 0.0f},
                                                  {m_ButtonSize.GetX(),  0.0f},
                                                  {m_ButtonSize.GetX(), -m_ButtonSize.GetY()},
                                                  {0.0f,                -m_ButtonSize.GetY()},
                                                  mousePos);
    }

    float GuiTextButton::GetWidth() const
    {
        return m_ButtonSize.GetX();
    }

    unsigned int GuiTextButton::GetGridDepth() const
    {
        return 1;
    }

    void GuiTextButton::Release()
    {
        GuiElement::Release();
        m_Callback();
    }

    void GuiTextButton::SetCallback(const std::function<void()>& callback)
    {
        m_Callback = callback;
    }

    void GuiTextButton::SetText(const std::string& text) {
        GuiText::SetText(text);
        m_ButtonSize = { GuiText::GetWidth() + GuiMetrics::ButtonTextMargin * 2.0f, m_ButtonSize.GetY() };
    }
}