#pragma once

#include <memory>

#include "GuiElement.h"
#include "GuiText.h"
#include "GuiButton.h"
#include "gui/styles/GuiStyling.h"
#include "core/Log.h"
#include "math/Vec2f.h"
#include "math/Math.h"
#include "core/Timestamp.h"

namespace Anwill {

    template <typename T>
    class GuiSlider : public GuiElement {
    public:
        std::shared_ptr<GuiStyling::Slider> m_Style;

        GuiSlider(const std::shared_ptr<GuiStyling::Container>& containerStyle, T min, T max, T& source,
                  const std::shared_ptr<GuiStyling::Slider>& style = nullptr)
            : GuiElement(containerStyle), m_Min(min), m_Max(max), m_Source(source),
              m_Style(style == nullptr ? std::make_shared<GuiStyling::Slider>() : style),
              m_ValueText(containerStyle, "", GuiStyling::Text::fontSize, style),
              m_Button(containerStyle, {containerStyle->elementHeight * 7.0f, containerStyle->elementHeight}, [](){},
                       style)
        {
            if(std::is_same<T, float>::value)
            {
                m_OnPressSet = [](float f) { return f; };
                m_SetValueText = [this](){ m_ValueText.Set(Math::RoundToString(m_Source, 2)); };
            }
            else if(std::is_same<T, int>::value)
            {
                m_OnPressSet = [](float f){ return std::round(f); };
                m_SetValueText = [this](){ m_ValueText.Set(std::to_string(m_Source)); };
            } else {
                // TODO: Double, long, etc
                AW_FATAL("GuiSlider template type not supported");
            }
        }

        void Render(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedMaxSize,
                    const Timestamp& delta) override
        {
            // Render background button
            m_Button.Render(assignedPos, assignedMaxSize, delta);

            // Render "slider" (marker) rectangle
            float markerXPosDelta = Math::ScaleToRange<float>(static_cast<float>(m_Source),
                                                              m_MarkerXOffset,
                                                              m_Button.GetWidth() - m_MarkerXOffset,
                                                              static_cast<float>(m_Min),
                                                              static_cast<float>(m_Max));
            markerXPosDelta = Math::Clamp(markerXPosDelta, m_MarkerXOffset, GetWidth() - m_MarkerXOffset);
            Math::Vec2f markerPos = {markerXPosDelta - m_MarkerXOffset,
                                     -(m_Button.GetHeight() -
                                       GuiStyling::Slider::GetMarkerSize(m_ContainerStyle->elementHeight).Y) * 0.5f + 1.0f}; // TODO: wtf end??

            GuiIcon::RenderRectangle(assignedPos + markerPos,
                                     GuiStyling::Slider::GetMarkerSize(m_ContainerStyle->elementHeight),
                                     m_Style->markerColor);

            // Render text
            m_SetValueText();
            float centeredTextXPos = m_Button.GetWidth() * 0.5f - m_ValueText.GetWidth() * 0.5f;
            m_ValueText.Render(assignedPos + Math::Vec2f(centeredTextXPos, 0.0f),
                               assignedMaxSize - Math::Vec2f(centeredTextXPos + GuiStyling::TextButton::textPadding, 0.0f),
                               delta);
        }

        bool IsHovering(const Math::Vec2f& mousePos) const override
        {
            return m_Button.IsHovering(mousePos);
        }

        float GetWidth() const override
        {
            return m_Button.GetWidth();
        }

        unsigned int GetGridDepth() const override
        {
            return 1;
        }

        void OnPress(const Math::Vec2f& mousePos) override {
            float t = Math::ScaleToRange<float>(mousePos.X,
                                                static_cast<float>(m_Min),
                                                static_cast<float>(m_Max),
                                                m_MarkerXOffset,
                                                GetWidth() - m_MarkerXOffset);
            t = Math::Clamp(t, static_cast<float>(m_Min), static_cast<float>(m_Max));
            m_Source = m_OnPressSet(t);
        }

    protected:
        const float m_MarkerXOffset = GuiStyling::Slider::markerWidth * 0.5f; // TODO

        T m_Min, m_Max;
        T& m_Source;
        GuiText m_ValueText;
        GuiButton m_Button;
        std::function<T(float)> m_OnPressSet;
        std::function<void()> m_SetValueText;
    };
}