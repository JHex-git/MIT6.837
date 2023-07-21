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
        Material(Vec3f{}), m_matrix(m), m_material1(mat1), m_material2(mat2) { }

    Vec3f getDiffuseColor(Vec3f position) const override;
    Vec3f getSpecularColor(Vec3f position) const override;
    Vec3f getReflectiveColor(Vec3f position) const override;
    Vec3f getTransparentColor(Vec3f position) const override;

    float getIndexOfRefraction() const override { return 1;}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;

    void glSetMaterial() const override { m_material1->glSetMaterial(); }

protected:
    virtual float getInterpolateCoefficient(Vec3f position) const = 0;

    std::shared_ptr<Matrix> m_matrix;
    Material *m_material1;
    Material *m_material2;
};
} // namespace materials

#endif