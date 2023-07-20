#include "materials/checker_board.h"
#include "raytrace/hit.h"

namespace materials
{

float CheckerBoard::getInterpolateCoefficient(Vec3f position) const
{
    Vec3f point = position;
    m_matrix->Transform(point);
    bool u = ((int)std::floor(point.x()) % 2) == 0;
    bool v = ((int)std::floor(point.y()) % 2) == 0;
    bool w = ((int)std::floor(point.z()) % 2) == 0;
    return u ^ v ^ w;
}
} // namespace materials
