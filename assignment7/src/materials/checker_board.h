#ifndef _CHECKER_BOARD_H_
#define _CHECKER_BOARD_H_

#include "materials/procedural_material.h"
#include "utility/matrix.h"
#include <memory>

namespace materials
{
using utility::Matrix;

class CheckerBoard : public ProceduralMaterial
{
public:
    CheckerBoard(Matrix *m, Material *mat1, Material *mat2) : ProceduralMaterial(m, mat1, mat2) { }

protected:
    float getInterpolateCoefficient(Vec3f position) const override;
};
} // namespace materials

#endif