#include "materials/noise.h"
#include "utility/perlin_noise.h"
#include "raytrace/hit.h"

namespace materials
{
using utility::PerlinNoise;

float Noise::getInterpolateCoefficient(Vec3f position) const
{
    auto point = position;
    m_matrix->Transform(point);
    double noise = PerlinNoise::octave_noise(point.x(), point.y(), point.z(), m_octaves);
    noise = std::min(1.0, noise + 0.5);
    noise = std::max(0.0, noise);
    return noise;
}
} // namespace materials
