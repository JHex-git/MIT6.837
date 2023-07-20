#ifndef _RANDOM_SAMPLER_H_
#define _RANDOM_SAMPLER_H_

#include "samples/sampler.h"
#include <random>

namespace samples
{

class RandomSampler : public Sampler
{
public:
    RandomSampler(int samples_per_pixel) : Sampler(samples_per_pixel), m_generator(std::random_device()()), m_distribution(0.f, 1.f) {}

    Vec2f getSamplePosition(int n) override
    {
        return Vec2f(m_distribution(m_generator), m_distribution(m_generator));
    }
private:
    std::mt19937 m_generator;
    std::uniform_real_distribution<float> m_distribution;
};
} // namespace samples
#endif