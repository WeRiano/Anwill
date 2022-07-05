#pragma once

#include <random>

namespace Anwill {

    class Random {
    public:
        static void Init();

        static float GetUniformFloat(float min, float max);
        static int GetUniformInt(int min, int max);
        static double GetUniformDouble(double min, double max);

    private:
        static std::default_random_engine s_RandEngine;
    };

}