#include "samples/filter.h"
#include "samples/film.h"

namespace samples
{

Vec3f Filter::getColor(int i, int j, Film *film)
{
    Vec3f color(0, 0, 0);
    float weight_sum = 0;
    for (int u = std::max(0, i - m_support_radius); u <= i + m_support_radius && u < film->getWidth(); ++u)
    {
        for (int v = std::max(0, j - m_support_radius); v <= j + m_support_radius && v < film->getHeight(); ++v)
        {
            for (int k = 0; k < film->getNumSamples(); ++k)
            {
                auto sample = film->getSample(u, v, k);
                float weight = getWeight(sample.getPosition().x() - 0.5 + u - i, sample.getPosition().y() + v - j - 0.5);
                color += sample.getColor() * weight;
                weight_sum += weight;
            }
        }
    }
    return color * (1 / weight_sum);
}

float BoxFilter::getWeight(float x, float y)
{
    if (std::abs(x) > m_radius || std::abs(y) > m_radius) return 0;
    return 1;
}

float TentFilter::getWeight(float x, float y)
{
    float r = std::sqrt(x * x + y * y);
    return std::max(1 - r / m_radius, 0.0f);
}

float GaussianFilter::getWeight(float x, float y)
{
    float r = std::sqrt(x * x + y * y);
    if (r > 2 * m_sigma) return 0;
    return std::exp(-r * r / (2 * m_sigma * m_sigma));
}
} // namespace samples
