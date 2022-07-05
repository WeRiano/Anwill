#pragma once

#include "gfx/Mesh.h"

namespace Anwill {

    struct MeshComponent {
        MeshComponent()
        {}
        Mesh mesh;
    };

    struct CameraComponent {
        int x, y, z;
        CameraComponent()
            : x(0), y(0), z(0)
        {}
        CameraComponent(int k)
            : x(0), y(k), z(0)
        {}

        void Identify() {
            std::cout << "CameraComponent" << std::endl;
        }
    };

    struct TransformComponent {

    };
}