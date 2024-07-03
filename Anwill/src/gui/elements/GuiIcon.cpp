#include "GuiIcon.h"

#include "gfx/Renderer2D.h"
#include "gui/styles/GuiStyling.h"

namespace Anwill {

    const std::array<GuiIcon::RenderIconFunctionType,
    (size_t) GuiStyling::Checkbox::CheckmarkType::Size> GuiIcon::renderIconFunctions = {
        GuiIcon::RenderCheckmark,
        GuiIcon::RenderRectangle,
        GuiIcon::RenderEllipse
    };

    void GuiIcon::RenderRightArrow(const Math::Vec2f& assignedPos,
                                   const Math::Vec2f& assignedSize,
                                   const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.X, assignedSize.Y, 1.0f});
        iconTransform = Math::Mat4f::RotateZ(iconTransform, 90);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + Math::Vec2f(assignedSize.X, -assignedSize.Y));

        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::triangleMesh,
                               iconTransform);
    }

    void GuiIcon::RenderDownArrow(const Math::Vec2f& assignedPos,
                                  const Math::Vec2f& assignedSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.X, assignedSize.Y, 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + Math::Vec2f(assignedSize.X, -assignedSize.Y));

        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::triangleMesh, iconTransform);
    }

    void GuiIcon::RenderCross(const Math::Vec2f& assignedPos,
                              const Math::Vec2f& assignedSize,
                              const Math::Vec3f& color) {
        // TODO
    }

    void GuiIcon::RenderCheckmark(const Math::Vec2f& assignedPos,
                                  const Math::Vec2f& assignedSize,
                                  const Math::Vec3f& color) {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(),
                                                       {assignedSize.X, assignedSize.Y, 1.0f});
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + (Math::Vec2f(assignedSize.X, -assignedSize.Y) * 0.5f));

        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::checkmarkMesh, iconTransform);
    }

    void GuiIcon::RenderRectangle(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedSize,
                                  const Math::Vec3f& color)
    {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), assignedSize);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + (Math::Vec2f(assignedSize.X,
                                                                             -assignedSize.Y) * 0.5f));

        GuiStyling::primitiveShader->Bind();
        GuiStyling::primitiveShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::primitiveShader, GuiStyling::rectMesh, iconTransform);
    }

    void GuiIcon::RenderEllipse(const Math::Vec2f& assignedPos, const Math::Vec2f& assignedSize,
                                const Math::Vec3f& color)
    {
        Math::Mat4f iconTransform = Math::Mat4f::Scale(Math::Mat4f::Identity(), assignedSize);
        iconTransform = Math::Mat4f::Translate(iconTransform, assignedPos
                                                              + (Math::Vec2f(assignedSize.X,
                                                                             -assignedSize.Y) * 0.5f));

        GuiStyling::circleShader->Bind();
        GuiStyling::circleShader->SetUniformVec3f(color, "u_Color");
        Renderer2D::SubmitMesh(GuiStyling::circleShader, GuiStyling::rectMesh, iconTransform);
    }
}