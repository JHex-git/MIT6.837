#ifndef _SAMPLER_H_
#define _SAMPLER_H_

#include <random>

namespace utility
{
    class Vec2f;
} // namespace utility

namespace samples
{
using utility::Vec2f;

class Sampler
{
public:
    Sampler(int samples_per_pixel) : m_samples_per_pixel(samples_per_pixel) {}

    virtual Vec2f getSamplePosition(int n) = 0;

    // ACCESSORS
    int getNumSamplesPerPixel() const { return m_samples_per_pixel; }

protected:
    int m_samples_per_pixel;
};

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

class UniformSampler : public Sampler
{
public:
    UniformSampler(int samples_per_pixel) : Sampler(samples_per_pixel) 
    {
        m_samples_per_row = (int)std::sqrt(samples_per_pixel);
        assert(std::sqrt(samples_per_pixel) == m_samples_per_row);
    }

    Vec2f getSamplePosition(int n) override
    {
        int x = n % m_samples_per_row;
        int y = n / m_samples_per_row;
        return Vec2f((x + 0.5f) / m_samples_per_row, (y + 0.5f) / m_samples_per_row);
    }
private:
    int m_samples_per_row;
};

class JitteredSampler : public Sampler
{
public:
    JitteredSampler(int samples_per_pixel) : Sampler(samples_per_pixel), m_distribution(0.f, 1.f), m_generator(std::random_device()())
    {
        m_samples_per_row = (int)std::sqrt(samples_per_pixel);
        assert(std::sqrt(samples_per_pixel) == m_samples_per_row);
    }

    Vec2f getSamplePosition(int n) override
    {
        int x = n % m_samples_per_row;
        int y = n / m_samples_per_row;
        return Vec2f((x + m_distribution(m_generator)) / m_samples_per_row, (y + m_distribution(m_generator)) / m_samples_per_row);
    }
private:
    int m_samples_per_row;
    std::mt19937 m_generator;
    std::uniform_real_distribution<float> m_distribution;
};

} // namespace samples


#endif