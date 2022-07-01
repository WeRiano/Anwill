#pragma once

namespace Anwill {

    class TestObject
    {
    public:
        TestObject(int i)
            : m_i(i)
        {}

        int m_i;
    };

    struct MeshComponent {
        int i;
        float f;
        MeshComponent()
            : i(0), f(0.0f)
        {}
        MeshComponent(int i, float f)
            : i(i), f(f)
        {}

        void Identify() {
            std::cout << "MeshComponent" << std::endl;
        }
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