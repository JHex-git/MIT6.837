#ifndef _PROCEDURAL_MATERIAL_H_
#define _PROCEDURAL_MATERIAL_H_

#include "materials/material.h"
#include "utility/matrix.h"
#include <memory>

namespace materials
{
using utility::Matrix;

class ProceduralMaterial : public Material
{
public:
    ProceduralMaterial(Matrix *m, Material *mat1, Material *mat2) : 
        Material(Vec3f(0, 0, 0)), m_matrix(m), m_material1(mat1), m_material2(mat2) { }

    Vec3f getSpecularColor() const override { return Vec3f(0, 0, 0);}
    Vec3f getReflectiveColor() const override { return Vec3f(0, 0, 0);}
    Vec3f getTransparentColor() const override { return Vec3f(0, 0, 0);}
    float getIndexOfRefraction() const override { return 1;}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override = 0;

    void glSetMaterial() const override { m_material1->glSetMaterial(); }

protected:
    std::shared_ptr<Matrix> m_matrix;
    Material *m_material1;
    Material *m_material2;
};
} // namespace materials

#endif