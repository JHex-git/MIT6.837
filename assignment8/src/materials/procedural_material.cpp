#include "materials/procedural_material.h"
#include "raytrace/hit.h"

namespace materials
{
using utility::Matrix;

Vec3f ProceduralMaterial::getDiffuseColor(Vec3f position) const
{
    float t = getInterpolateCoefficient(position);
    return t * m_material1->getDiffuseColor(position) + (1 - t) * m_material2->getDiffuseColor(position);
}
Vec3f ProceduralMaterial::getSpecularColor(Vec3f position) const
{
    float t = getInterpolateCoefficient(position);
    return t * m_material1->getSpecularColor(position) + (1 - t) * m_material2->getSpecularColor(position);
}
Vec3f ProceduralMaterial::getReflectiveColor(Vec3f position) const
{
    float t = getInterpolateCoefficient(position);
    return t * m_material1->getReflectiveColor(position) + (1 - t) * m_material2->getReflectiveColor(position);
}
Vec3f ProceduralMaterial::getTransparentColor(Vec3f position) const
{
    float t = getInterpolateCoefficient(position);
    return t * m_material1->getTransparentColor(position) + (1 - t) * m_material2->getTransparentColor(position);
}

Vec3f ProceduralMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    float t = getInterpolateCoefficient(hit.getIntersectionPoint());
    return t * m_material1->Shade(ray, hit, dirToLight, lightColor) + (1 - t) * m_material2->Shade(ray, hit, dirToLight, lightColor);
}
} // namespace materials
