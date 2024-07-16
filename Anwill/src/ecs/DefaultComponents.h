#pragma once

#include "gfx/Mesh.h"
#include "math/Mat4f.h"

namespace Anwill {

    // TODO: Do we need these?? Whats the difference between for example a
    //  transform component vs. just using the mat4f directly?

    /*
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
    */
}