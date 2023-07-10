#ifndef _PHONG_MATERIAL_H_
#define _PHONG_MATERIAL_H_
#include "materials/material.h"

namespace materials
{
class PhongMaterial : public Material
{
public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent)
        : Material(diffuseColor), m_specularColor(specularColor), m_exponent(exponent) {}

    void glSetMaterial(void) const override;
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;

private:
    Vec3f m_specularColor;
    float m_exponent;
};
} // namespace materials

#endif