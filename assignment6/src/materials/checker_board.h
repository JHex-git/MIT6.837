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

    Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const override;
};
} // namespace materials

#endif