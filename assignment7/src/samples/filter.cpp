#include "samples/filter.h"
#include "samples/film.h"

namespace samples
{


Vec3f Filter::getColor(int i, int j, Film *film)
{
    Vec3f color(0, 0, 0);
    float weight_sum = 0;
    for (int k = 0; k < film->getNumSamples(); ++k)
    {
        auto sample = film->getSample(i, j, k);
        float weight = getWeight(sample.getPosition().x(), sample.getPosition().y());
        color += sample.getColor() * weight;
        weight_sum += weight;
    }
    return color * (1 / weight_sum);
}

float BoxFilter::getWeight(float x, float y)
{
    return 1.f / (2 * getSupportRadius() + 1);
}

float TentFilter::getWeight(float x, float y)
{
    float r = std::sqrt(x * x + y * y);
    if (r > getSupportRadius()) return 0;
    return 1 - r / getSupportRadius();
}

float GaussianFilter::getWeight(float x, float y)
{
    float r = std::sqrt(x * x + y * y);
    if (r > 2 * m_sigma) return 0;
    return std::exp(-r * r / (2 * m_sigma * m_sigma));
}
} // namespace samples
