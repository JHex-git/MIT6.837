#ifndef _WOOD_H_
#define _WOOD_H_

#include "materials/procedural_material.h"
#include <memory>

namespace utility
{
    class Matrix;
} // namespace utility


namespace materials
{
using utility::Matrix;

class Wood : public ProceduralMaterial
{
public:
    Wood(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) : 
        ProceduralMaterial(m, mat1, mat2), m_octaves(octaves), m_frequency(frequency), m_amplitude(amplitude) {}

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;
    
private:
    int m_octaves;
    float m_frequency;
    float m_amplitude;
};
} // namespace materials

#endif