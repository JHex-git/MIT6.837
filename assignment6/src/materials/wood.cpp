#include "materials/wood.h"
#include "utility/perlin_noise.h"
#include "raytrace/hit.h"

namespace materials
{
using utility::PerlinNoise;

Vec3f Wood::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    auto point = hit.getIntersectionPoint();
    m_matrix->Transform(point);
    double noise = PerlinNoise::octave_noise(point.x(), point.y(), point.z(), m_octaves);
    double t = std::sin(m_frequency * (std::sqrt(point.z() * point.z() + point.x() * point.x())) + m_amplitude * noise + point.y() * 0.5);
    t = t / 2 + 0.5;
    return t * m_material1->Shade(ray, hit, dirToLight, lightColor) + (1 - t) * m_material2->Shade(ray, hit, dirToLight, lightColor);
}

}