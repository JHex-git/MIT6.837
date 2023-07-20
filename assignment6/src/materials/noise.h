#ifndef _NOISE_H_
#define _NOISE_H_
#include "materials/procedural_material.h"

namespace materials
{

class Noise : public ProceduralMaterial
{
public:
    Noise(Matrix *m, Material *mat1, Material *mat2, int octaves) : ProceduralMaterial(m, mat1, mat2), m_octaves(octaves) { }

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;

private:
    int m_octaves;
};
} // namespace materials
#endif