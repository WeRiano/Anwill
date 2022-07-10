#pragma once

#include "gfx/Mesh.h"
#include "math/Mat4f.h"

namespace Anwill {

    struct MeshComponent {
        MeshComponent()
        {}
        Mesh mesh;
    };

    struct TransformComponent {
        TransformComponent()
            : transform(Math::Mat4f::Identity())
        {}
        Math::Mat4f transform;
    };
}