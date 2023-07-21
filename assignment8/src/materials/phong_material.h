#ifndef _PHONG_MATERIAL_H_
#define _PHONG_MATERIAL_H_
#include "materials/material.h"

namespace materials
{
class PhongMaterial : public Material
{
public:
    PhongMaterial(const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent, 
                    const Vec3f &reflectiveColor, const Vec3f &transparentColor, float indexOfRefraction)
        : Material(diffuseColor), m_specularColor(specularColor), m_exponent(exponent), 
            m_reflectiveColor(reflectiveColor), m_transparentColor(transparentColor), m_indexOfRefraction(indexOfRefraction) {}

    void glSetMaterial(void) const override;
    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;
    
    inline Vec3f getSpecularColor(Vec3f position) const override { return m_specularColor; }
    inline Vec3f getReflectiveColor(Vec3f position) const override { return m_reflectiveColor; }
    inline Vec3f getTransparentColor(Vec3f position) const override { return m_transparentColor; }
    inline float getIndexOfRefraction() const override { return m_indexOfRefraction; }

private:
    float m_exponent;
    float m_indexOfRefraction;
    Vec3f m_specularColor;
    Vec3f m_reflectiveColor;
    Vec3f m_transparentColor;
};
} // namespace materials

#endif