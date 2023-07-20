#ifndef _NOISE_H_
#define _NOISE_H_
#include "materials/procedural_material.h"

namespace materials
{

class Noise : public ProceduralMaterial
{
public:
    Noise(Matrix *m, Material *mat1, Material *mat2, int octaves) : ProceduralMaterial(m, mat1, mat2), m_octaves(octaves) { }

protected:
    float getInterpolateCoefficient(Vec3f position) const override;

private:
    int m_octaves;
};
} // namespace materials
#endif