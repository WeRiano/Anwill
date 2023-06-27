#include "gui/GuiElements.h"
#include "math/Algo.h"
#include "core/Log.h"
#include "utils/Utils.h"

namespace Anwill {

    // ---------- HELPERS ----------

    Math::Vec2f GetCutoffPos(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        return {assignedPos.GetX() + assignedMaxSize.GetX(), assignedPos.GetY() - assignedMaxSize.GetY()};
    }

    /**
     * @brief Get the position of the next horizontal or vertical element in a container
     */
    Math::Vec2f GetNextElementPos(const Math::Vec2f& curPos, float currentElementWidth,
                                  unsigned int currentElementGridDepth, float originXPos,
                                  bool onNewRow)
    {
        if(onNewRow) {
            return {
                    originXPos,
                    curPos.GetY() - ((float) currentElementGridDepth * (GuiStyling::windowElementHeight +
                                                                        GuiStyling::windowElementVerticalMargin))
            };
        } else {
            return {
                    curPos.GetX() + currentElementWidth + GuiStyling::windowElementHorizontalMargin,
                    curPos.GetY()
            };
        }
    }

    /**
     * @brief Get the maximum allowed width and height of an element given its position and
     *        the current maximum width and height.
     */
    Math::Vec2f GetNewMaxSize(const Math::Vec2f& posDelta,
                              const Math::Vec2f& oldMaxSize) {
        return {oldMaxSize.GetX() - posDelta.GetX(), oldMaxSize.GetY() + posDelta.GetY()};
    }

    // ---------- ICON ----------

    void GuiIcon::RenderRightArrow(const Math::Vec2f& assignedPos,
                                   const Math::Vec2f& assignedSize,
                                   const Math::Vec2f& assignedMaxSize,
                                   const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::RotateZ(iconTransform, 90);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()));

        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiStyling::primitiveShader, GuiStyling::triangleMesh, iconTransform);
    }

    void GuiIcon::RenderDownArrow(const Math::Vec2f& assignedPos,
                                  const Math::Vec2f& assignedSize,
                                  const Math::Vec2f& assignedMaxSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()));

        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiStyling::primitiveShader, GuiStyling::triangleMesh, iconTransform);
    }

    void GuiIcon::RenderCross(const Math::Vec2f& assignedPos,
                              const Math::Vec2f& assignedSize,
                              const Math::Vec2f& assignedMaxSize,
                              const Math::Vec3f& color) {
        // TODO
    }

    void GuiIcon::RenderCheckmark(const Math::Vec2f &assignedPos,
                                  const Math::Vec2f &assignedSize,
                                  const Math::Vec2f &assignedMaxSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.GetX(), assignedSize.GetY(), 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
        + (Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()) * 0.5f));

        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiStyling::primitiveShader, GuiStyling::checkmarkMesh, iconTransform);
    }

    void GuiIcon::RenderRectangle(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedSize,
                                  const Math::Vec2f& assignedMaxSize, const Math::Vec3f& color)
    {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), assignedSize);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + (Math::Vec2f(assignedSize.GetX(), -assignedSize.GetY()) * 0.5f));

        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::Submit(GuiStyling::primitiveShader, GuiStyling::rectMesh, iconTransform);
    }

    // ---------- ELEMENT ----------

    GuiElement::GuiElement()
        : m_IsHovered(false), m_IsPressed(false)
    {}

    void GuiElement::StartHovering()
    {
        m_IsHovered = true;
    }

    void GuiElement::StopHovering()
    {
        m_IsHovered = false;
    }

    void GuiElement::OnHover(const Math::Vec2f& mousePos)
    {
        //AW_INFO("Hovering!");
        // Default behavior is nothing
    }

    void GuiElement::StartPressing()
    {
        //AW_INFO("Start pressing ...");
        m_IsPressed = true;
    }

    void GuiElement::OnPress(const Math::Vec2f& mousePos)
    {
        //AW_INFO("Pressing!");
        // Default behavior is nothing
    }

    void GuiElement::Release()
    {
        //AW_INFO("Stop pressing ...");
        m_IsPressed = false;
    }

    bool GuiElement::IsHovered() const
    {
        return m_IsHovered;
    }

    // ---------- TEXT ----------

    GuiText::GuiText(const std::string& text, unsigned int textSize)
        : GuiElement(),
          m_TextPos(1.0f, -GuiStyling::windowElementHeight / 2.0f + GuiStyling::textBaselineOffset),
          m_Text(text),
          m_TextScale(Font::GetScaleValue(textSize)), m_TextWidth((float) GuiStyling::font->GetStringWidth(text) * m_TextScale)
    {}

    void GuiText::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f cutoffPos = GetCutoffPos(Math::Vec2f(m_TextPos.GetX(), 0.0f) + assignedPos, assignedMaxSize);
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                {m_TextScale, m_TextScale, 1.0f});
        thisTransform = Math::Mat4f::Translate(thisTransform, m_TextPos + assignedPos);

        // Render
        GuiStyling::textShader->Bind();
        GuiStyling::textShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiStyling::textShader, *GuiStyling::font, m_Text, thisTransform);
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
        m_TextWidth = (float) GuiStyling::font->GetStringWidth(text) * m_TextScale;
    }

    // ---------- BUTTON ----------

    GuiButton::GuiButton(const Math::Vec2f& size, const std::function<void()>& callback)
        : GuiElement(),
          m_ButtonSize(size),
          m_Callback(callback)
    {}

    void GuiButton::Render(const Math::Vec2f &assignedPos, const Math::Vec2f &assignedMaxSize) {
        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        auto thisTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), m_ButtonSize);
        thisTransform = Math::Mat4f::Translate(thisTransform,
                                               assignedPos + Math::Vec2f(m_ButtonSize.GetX() / 2.0f, -m_ButtonSize.GetY() / 2.0f));

        // Render button
        GuiStyling::textButtonShader->Bind();
        GuiStyling::textButtonShader->SetUniform1i(m_IsHovered, "u_Hovering");
        GuiStyling::textButtonShader->SetUniform1i(m_IsPressed, "u_Pressing");
        GuiStyling::textButtonShader->SetUniformVec2f(cutoffPos, "u_CutoffPos");
        Renderer2D::Submit(GuiStyling::textButtonShader, GuiStyling::rectMesh, thisTransform);
    }

    bool GuiButton::IsHovering(const Math::Vec2f& mousePos) const
    {
        return Math::Algo::IsPointInsideRectangle({0.0f,                 0.0f},
                                                  {m_ButtonSize.GetX(),  0.0f},
                                                  {m_ButtonSize.GetX(), -m_ButtonSize.GetY()},
                                                  {0.0f,                -m_ButtonSize.GetY()},
                                                  mousePos);
    }

    float GuiButton::GetWidth() const
    {
        return m_ButtonSize.GetX();
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

    // ---------- TEXT BUTTON ----------

    GuiTextButton::GuiTextButton(const std::string& text, unsigned int textSize,
                                 const std::function<void()>& callback)
        : GuiButton({}, callback),
          m_Text(text, textSize)
    {
        m_ButtonSize = {m_Text.GetWidth() + GuiStyling::buttonTextPadding * 2.0f, GuiStyling::windowElementHeight};
    }

    void GuiTextButton::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render text
        m_Text.Render(assignedPos + Math::Vec2f(GuiStyling::buttonTextPadding, 0.0f),
                      assignedMaxSize - Math::Vec2f(GuiStyling::buttonTextPadding, 0.0f));
    }

    void GuiTextButton::SetText(const std::string& text) {
        m_Text.SetText(text);
        m_ButtonSize = { m_Text.GetWidth() + GuiStyling::buttonTextPadding * 2.0f, m_ButtonSize.GetY() };
    }

    // ---------- CHECKBOX ----------

    GuiCheckbox::GuiCheckbox(bool startAsChecked,
                             const std::function<void(bool)>& callback)
        : GuiButton({GuiStyling::windowElementHeight, GuiStyling::windowElementHeight},
                    [this, callback](){
            if(m_IsHovered) {
                m_Checked = !m_Checked;
                callback(m_Checked);
            }
        }), m_Checked(startAsChecked)
    {}

    void GuiCheckbox::Render(const Math::Vec2f &assignedPos, const Math::Vec2f &assignedMaxSize) {
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render checkmark
        if(!m_Checked) { return; }
        GuiIcon::RenderCheckmark(assignedPos + Math::Vec2f(GuiStyling::checkboxElementMargin, -GuiStyling::checkboxElementMargin * 2.0f),
                                 {m_ButtonSize.GetX() - GuiStyling::checkboxElementMargin * 2.0f,
                                  m_ButtonSize.GetY() - GuiStyling::checkboxElementMargin * 4.0f},
                                 assignedMaxSize,
                                 {1.0f, 1.0f, 1.0f});
    }

    // ---------- SLIDER ----------

    GuiSlider::GuiSlider()
        : GuiButton({GuiStyling::windowElementHeight * 7.0f, GuiStyling::windowElementHeight}, [](){}),
          m_ValueText("", GuiStyling::fontSize)
    {}

    void GuiSlider::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        // Render background button
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render "slider" (marker) rectangle
        Math::Vec2f markerPos = {m_LastCursorXPos - GuiStyling::sliderMarkerSize.GetX() * 0.5f,
                                 -(m_ButtonSize.GetY() - GuiStyling::sliderMarkerSize.GetY()) * 0.5f + 1.0f};
        markerPos = {Utils::Clamp(markerPos.GetX(), 0.0f, GetWidth() - GuiStyling::sliderMarkerSize.GetX()),
                     markerPos.GetY()};
        GuiIcon::RenderRectangle(assignedPos + markerPos, GuiStyling::sliderMarkerSize,
                                 assignedMaxSize - markerPos, {0.45f, 0.45f, 1.0f});

        // Render text
        float centeredTextXPos = m_ButtonSize.GetX() * 0.5f - m_ValueText.GetWidth() * 0.5f;
        m_ValueText.Render(assignedPos + Math::Vec2f(centeredTextXPos, 0.0f),
                      assignedMaxSize - Math::Vec2f(GuiStyling::buttonTextPadding, 0.0f));
    }

    void GuiSlider::OnPress(const Math::Vec2f &mousePos) {
        GuiElement::OnPress(mousePos);

        m_LastCursorXPos = mousePos.GetX();
    }

    // ---------- INT SLIDER ----------

    GuiIntSlider::GuiIntSlider(int min, int max, int* sliderValue)
        : m_Min(min), m_Max(max), m_ClientValuePointer(sliderValue)
    {
        int startValue = m_Min;
        m_ValueText.SetText(std::to_string(startValue));
        m_LastCursorXPos = 0.0f;
    }

    void GuiIntSlider::OnPress(const Math::Vec2f &mousePos) {
        GuiSlider::OnPress(mousePos);

        m_LastCursorXPos = mousePos.GetX();
        float sliderValue = Utils::ScaleToRange(mousePos.GetX(),
                                                static_cast<float>(m_Min),
                                                static_cast<float>(m_Max),
                                                GuiStyling::sliderMarkerSize.GetX() * 0.5f,
                                                GetWidth() - GuiStyling::sliderMarkerSize.GetX() * 0.5f);
        sliderValue = Utils::Clamp(sliderValue,
                                   static_cast<float>(m_Min),
                                   static_cast<float>(m_Max));
        int roundedInt = Utils::RoundToInt(sliderValue);
        *m_ClientValuePointer = roundedInt;
        m_ValueText.SetText(std::to_string(roundedInt));
    }

    // ---------- FLOAT SLIDER ----------

    GuiFloatSlider::GuiFloatSlider(float min, float max, float* sliderValue)
        : m_Min(min), m_Max(max), m_ClientValuePointer(sliderValue)
    {
        float startValue = m_Min;
        m_ValueText.SetText(Utils::RoundFloatToString(startValue, 3));
        m_LastCursorXPos = 0.0f;
    }

    void GuiFloatSlider::OnPress(const Math::Vec2f& mousePos) {
        GuiSlider::OnPress(mousePos);

        float sliderValue = Utils::ScaleToRange(mousePos.GetX(),
                                                static_cast<float>(m_Min),
                                                static_cast<float>(m_Max),
                                                GuiStyling::sliderMarkerSize.GetX() * 0.5f,
                                                GetWidth() - GuiStyling::sliderMarkerSize.GetX() * 0.5f);
        sliderValue = Utils::Clamp(sliderValue,
                                   static_cast<float>(m_Min),
                                   static_cast<float>(m_Max));
        *m_ClientValuePointer = sliderValue;
        m_ValueText.SetText(Utils::RoundFloatToString(sliderValue, 3));
    }

    // -------------- TOOLTIP --------------

    GuiTooltip::GuiTooltip(const std::string& text, unsigned int textSize, const std::string& tooltipText,
                           unsigned int tooltipTextSize)
       : GuiText(text, textSize), m_TooltipText(tooltipText, tooltipTextSize)
    {

    }

    void GuiTooltip::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        Math::Vec2f tooltipWindowSize = GetNewMaxSize(assignedPos, assignedMaxSize);


        GuiStyling::tooltipShader->Bind();
        //auto transform = Math::Mat4f::Translate(Math::Mat4f::Identity(), assignedPos +
        //GuiStyling::tooltipOffset + Math::Vec2f(.GetX() / 2.0f, -m_Size.GetY() / 2.0f));
        //transform = Math::Mat4f::Scale(transform, m_Size);
        //Renderer2D::Submit(GuiStyling::tooltipShader, GuiStyling::rectMesh, transform);

        //GuiText::Render(assignedPos, assignedMaxSize);
    }

    void GuiTooltip::OnHover(const Math::Vec2f& mousePos)
    {
        GuiElement::OnHover(mousePos);
    }

    // ------------- CONTAINER -------------

    GuiContainer::GuiContainer()
            : m_GridDepth(1), m_HideElements(true)
    {}

    std::shared_ptr<GuiElement> GuiContainer::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                              const Math::Vec2f& mousePos) const
    {
        if(m_HideElements) { return nullptr; }
        for(unsigned int i = 0; i < m_Elements.size(); i++) {
            auto& element = m_Elements[i];
            if (element->IsHovering(mousePos - (m_ElementPosCache[i]))) {
                hoverElementPos = m_ElementPosCache[i];
                return element;
            }
            if (dynamic_cast<GuiContainer *>(element.get()) != nullptr) {
                // If element is a container we need to check with those elements
                auto container = std::dynamic_pointer_cast<GuiContainer>(element);
                Math::Vec2f elementPosInsideContainer = {};
                auto maybeResult = container->GetHoverElement(
                        elementPosInsideContainer,
                        mousePos - (m_ElementPosCache[i]));
                if (maybeResult != nullptr) {
                    // If we found an element we return it, otherwise we continue
                    hoverElementPos = m_ElementPosCache[i] + elementPosInsideContainer;
                    return maybeResult;
                }
            }
        }
        return nullptr;
    }

    void GuiContainer::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                              const Math::Vec2f& firstPos)
    {
        m_ElementPosCache.clear();
        Math::Vec2f elementGridPos = firstPos;
        float newRowXPos = firstPos.GetX();
        std::shared_ptr<GuiElement> lastElement = nullptr;
        for (unsigned int i = 0; i < m_Elements.size(); i++) {
            auto element = m_Elements[i];
            if(i > 0) {
                bool wantsNewRow = m_NewRowChecks[i].first;
                bool forcedToNewRow = m_NewRowChecks[i].second;
                elementGridPos = GetNextElementPos(elementGridPos, lastElement->GetWidth(),
                                                   lastElement->GetGridDepth(),
                                                   newRowXPos,
                                                   wantsNewRow || forcedToNewRow);
            }
            element->Render(assignedPos + elementGridPos,
                            GetNewMaxSize(elementGridPos + Math::Vec2f(GuiStyling::windowCutoffPadding,
                                                                       GuiStyling::windowCutoffPadding),
                                          assignedMaxSize));
            m_ElementPosCache.push_back(elementGridPos);
            lastElement = element;
        }
    }

    bool GuiContainer::IsHidingElements() const
    {
        return m_HideElements;
    }

    // ------------- DROPDOWN -------------

    GuiDropdown::GuiDropdown(const std::string& text, unsigned int textSize)
            : GuiTextButton(text, textSize, [this](){
        m_HideElements = !m_HideElements;
    })
    {}

    void GuiDropdown::Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize)
    {
        // Force button size to max width render it
        m_ButtonSize = { assignedMaxSize.GetX(), m_ButtonSize.GetY()};
        GuiButton::Render(assignedPos, assignedMaxSize);

        // Render arrow icon
        Math::Vec2f cutoffPos = GetCutoffPos(assignedPos, assignedMaxSize);
        if(m_HideElements) {
            GuiIcon::RenderRightArrow(assignedPos,
                                      GuiStyling::iconSize * 0.5f,
                                      assignedMaxSize,
                                      {1.0f, 1.0f, 1.0f});
        } else {
            GuiIcon::RenderDownArrow(assignedPos,
                                     GuiStyling::iconSize * 0.5f,
                                     assignedMaxSize,
                                     {1.0f, 1.0f, 1.0f});
        }

        // Render text slightly to the right compared to a regular text button
        m_Text.Render(assignedPos + Math::Vec2f(GuiStyling::iconSize.GetX(), 0.0f),
                      assignedMaxSize - Math::Vec2f(GuiStyling::iconSize.GetX(), 0.0f));

        if(m_HideElements) { return; }
        GuiContainer::Render(assignedPos, assignedMaxSize,
                             { GuiStyling::windowElementIndent,
                               -GuiStyling::windowElementHeight - GuiStyling::windowElementVerticalMargin });
    }

    bool GuiDropdown::IsHovering(const Math::Vec2f& mousePos) const
    {
        if (GuiTextButton::IsHovering(mousePos)) {
            return true;
        } else {
            for(unsigned int i = 0; i < m_Elements.size(); i++) {
                if(m_Elements[i]->IsHovering(mousePos)) {
                    return true;
                }
            }
        }
        return false;
    }

    float GuiDropdown::GetWidth() const
    {
        return GuiTextButton::GetWidth();
    }

    unsigned int GuiDropdown::GetGridDepth() const
    {
        return m_HideElements ? 1 : m_GridDepth;
    }

    // -------- WINDOW --------

    GuiWindow::GuiWindow(const std::string& title, GuiWindowID id, const Math::Vec2f& position, const Math::Vec2f& size)
            : GuiContainer(), m_Pos(position), m_Size(size), m_LastShowSize(), m_ID(id),
              m_Title(title, 14),
              m_MinimizeButton(std::make_shared<GuiButton>(GuiStyling::iconSize,
                                                           [this]() {
                                                               m_HideElements = !m_HideElements;
                                                               if(m_HideElements) {
                                                                   m_LastShowSize = m_Size;
                                                                   m_Size = {m_Size.GetX(),
                                                                             GuiStyling::windowHeaderSize};
                                                               } else {
                                                                   m_Size = m_LastShowSize;
                                                               }
                                                           }))
    {
        m_HideElements = false;
    }

    std::shared_ptr<GuiElement> GuiWindow::GetHoverElement(Math::Vec2f& hoverElementPos,
                                                           const Math::Vec2f& mousePos) const
    {
        // Two things in addition to GuiContainer implementation:
        //  1. We have a static minimize button element that we have to consider
        //  2. We have to consider the position of the window since it is the "outer" most container
        if(m_MinimizeButton->IsHovering(mousePos - m_Pos)) {
            return m_MinimizeButton;
        }
        auto hoverElement = GuiContainer::GetHoverElement(hoverElementPos, mousePos - m_Pos);
        hoverElementPos = hoverElementPos + m_Pos;
        return hoverElement;
    }

    void GuiWindow::Render(bool selected)
    {
        // Render window
        GuiStyling::windowShader->Bind();
        GuiStyling::windowShader->SetUniform1i(selected, "u_Selected");
        auto transform = Math::Mat4f::Translate(Math::Mat4f::Identity(),
                                                m_Pos + Math::Vec2f(m_Size.GetX() / 2.0f, -m_Size.GetY() / 2.0f));
        transform = Math::Mat4f::Scale(transform, m_Size);
        Renderer2D::Submit(GuiStyling::windowShader, GuiStyling::rectMesh, transform);

        // Render title
        m_Title.Render(m_Pos + GuiStyling::windowTitlePos, m_Size - GuiStyling::windowTitlePos
                                                           - Math::Vec2f(GuiStyling::windowCutoffPadding, GuiStyling::windowCutoffPadding));

        // Render minimize button
        m_MinimizeButton->Render(m_Pos, m_Size);
        if(m_HideElements) {
            GuiIcon::RenderRightArrow(m_Pos,
                                      GuiStyling::iconSize * 0.5f,
                                      m_Size - Math::Vec2f(GuiStyling::windowCutoffPadding,
                                                           GuiStyling::windowCutoffPadding),
                                      {1.0f, 1.0f, 1.0f});
            return;
        } else {
            GuiIcon::RenderDownArrow(m_Pos,
                                     GuiStyling::iconSize * 0.5f,
                                     m_Size - Math::Vec2f(GuiStyling::windowCutoffPadding, GuiStyling::windowCutoffPadding),
                                     {1.0f, 1.0f, 1.0f});
        }


        GuiContainer::Render(m_Pos, m_Size,
                             {GuiStyling::windowElementIndent,
                              -(GuiStyling::windowHeaderSize + GuiStyling::windowElementVerticalMargin) });
    }

    bool GuiWindow::IsHoveringHeader(const Math::Vec2f& mousePos)
    {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX() + GuiStyling::windowTitlePos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + m_Size.GetX(), m_Pos.GetY() - GuiStyling::windowHeaderSize},
                                                  {m_Pos.GetX() + GuiStyling::windowTitlePos.GetX(), m_Pos.GetY() - GuiStyling::windowHeaderSize},
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

    bool GuiWindow::IsHoveringMinimize(const Math::Vec2f& mousePos) {
        return Math::Algo::IsPointInsideRectangle({m_Pos.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + GuiStyling::iconSize.GetX(), m_Pos.GetY()},
                                                  {m_Pos.GetX() + GuiStyling::iconSize.GetX(), m_Pos.GetY()
                                                                                               - GuiStyling::windowHeaderSize},
                                                  {m_Pos.GetX(), m_Pos.GetY() - GuiStyling::windowHeaderSize},
                                                  mousePos);
    }

    void GuiWindow::Move(const Math::Vec2f& delta, const Math::Vec2f& minPos, const Math::Vec2f& maxPos)
    {
        if(!(m_Pos.GetY() + delta.GetY() > maxPos.GetY() ||
             m_Pos.GetY() - GuiStyling::windowHeaderSize + delta.GetY() < minPos.GetY())) {
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

    Math::Vec2f GuiWindow::GetPos() const
    {
        return m_Pos;
    }

    GuiWindowID GuiWindow::GetID() const
    {
        return m_ID;
    }
}