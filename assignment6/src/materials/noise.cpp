#include "materials/noise.h"
#include "utility/perlin_noise.h"
#include "raytrace/hit.h"

namespace materials
{
using utility::PerlinNoise;

Vec3f Noise::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    auto point = hit.getIntersectionPoint();
    m_matrix->Transform(point);
    double noise = PerlinNoise::octave_noise(point.x(), point.y(), point.z(), m_octaves);
    noise = std::min(1.0, noise + 0.5);
    noise = std::max(0.0, noise);

    return noise * m_material1->Shade(ray, hit, dirToLight, lightColor) + (1 - noise) * m_material2->Shade(ray, hit, dirToLight, lightColor);
}
} // namespace materials
