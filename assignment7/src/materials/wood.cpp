#include "materials/wood.h"
#include "utility/perlin_noise.h"
#include "raytrace/hit.h"

namespace materials
{
using utility::PerlinNoise;

float Wood::getInterpolateCoefficient(Vec3f position) const
{
    auto point = position;
    m_matrix->Transform(point);
    double noise = PerlinNoise::octave_noise(point.x(), point.y(), point.z(), m_octaves);
    double t = std::sin(m_frequency * (std::sqrt(point.z() * point.z() + point.x() * point.x())) + m_amplitude * noise + point.y() * 0.5);
    t = t / 2 + 0.5;
    return t;
}
}