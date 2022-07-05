#include "Random.h"
#include "core/Timestamp.h"

namespace Anwill {

    std::default_random_engine Random::s_RandEngine;

    void Random::Init()
    {
        s_RandEngine = std::default_random_engine(Timestamp().GetMicroseconds());
    }

    float Random::GetUniformFloat(float min, float max)
    {
        s_RandEngine.discard(1);
        std::uniform_real_distribution<float> distribution(min, max);
        return distribution(s_RandEngine);
    }

    int Random::GetUniformInt(int min, int max)
    {
        s_RandEngine.discard(1);
        std::uniform_int_distribution<int> distribution(min, max);
        return distribution(s_RandEngine);
    }

    double Random::GetUniformDouble(double min, double max)
    {
        s_RandEngine.discard(1);
        std::uniform_real_distribution<double> distribution(min, max);
        return distribution(s_RandEngine);
    }
}