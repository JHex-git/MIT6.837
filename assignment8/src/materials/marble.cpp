#include "materials/marble.h"
#include "utility/perlin_noise.h"
#include "raytrace/hit.h"

namespace materials
{
using utility::PerlinNoise;

float Marble::getInterpolateCoefficient(Vec3f position) const
{
    auto point = position;
    m_matrix->Transform(point);
    double noise = PerlinNoise::octave_noise(point.x(), point.y(), point.z(), m_octaves);
    double t = std::sin(m_frequency * point.x() + m_amplitude * noise);
    t = t / 2 + 0.5;
    return t;
}
}