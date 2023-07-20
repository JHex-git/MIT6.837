#ifndef _MARBLE_H_
#define _MARBLE_H_

#include "materials/procedural_material.h"
#include <memory>

namespace materials
{
    
class Marble : public ProceduralMaterial
{
public:
    Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) : 
        ProceduralMaterial(m, mat1, mat2), m_octaves(octaves), m_frequency(frequency), m_amplitude(amplitude) {}

protected:
    float getInterpolateCoefficient(Vec3f position) const override;

private:
    int m_octaves;
    float m_frequency;
    float m_amplitude;
};
} // namespace materials

#endif