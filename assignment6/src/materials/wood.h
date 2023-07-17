#ifndef _WOOD_H_
#define _WOOD_H_

#include "materials/material.h"
#include <memory>

namespace utility
{
    class Matrix;
} // namespace utility


namespace materials
{
using utility::Matrix;

class Wood : public Material
{
public:
    Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude);

    Vec3f getSpecularColor() const override { return Vec3f(0, 0, 0);}
    Vec3f getReflectiveColor() const override { return Vec3f(0, 0, 0);}
    Vec3f getTransparentColor() const override { return Vec3f(0, 0, 0);}
    float getIndexOfRefraction() const override { return 1;}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;

    void glSetMaterial() const override;
    
private:
    std::shared_ptr<Matrix> m_matrix;
    Material *m_material1;
    Material *m_material2;
    int m_octaves;
    float m_frequency;
    float m_amplitude;
};
} // namespace materials

#endif