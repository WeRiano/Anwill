#pragma once

#include "Component.h"

namespace Anwill {

    class TestObject
    {
    public:
        TestObject(int i)
            : m_i(i)
        {}

        int m_i;
    };

    struct MeshComponent : public Component {
        int i;
        float f;
        TestObject obj;
        MeshComponent(int i, float f, TestObject obj)
            : i(i), f(f), obj(obj)
        {}

        void Identify() override {
            std::cout << "MeshComponent" << std::endl;
        }
    };

    struct CameraComponent : public Component {
        int x, y, z;
        CameraComponent(int k)
            : x(0), y(k), z(0)
        {}

        void Identify() override {
            std::cout << "CameraComponent" << std::endl;
        }
    };

    struct TransformComponent : public Component {

    };
}