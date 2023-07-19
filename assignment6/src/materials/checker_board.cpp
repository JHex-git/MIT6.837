#include "materials/checker_board.h"
#include "raytrace/hit.h"

namespace materials
{

Vec3f CheckerBoard::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    Vec3f point = hit.getIntersectionPoint();
    m_matrix->Transform(point);
    bool u = ((int)std::floor(point.x()) % 2) == 0;
    bool v = ((int)std::floor(point.y()) % 2) == 0;
    bool w = ((int)std::floor(point.z()) % 2) == 0;
    if (u ^ v ^ w)
        return m_material1->Shade(ray, hit, dirToLight, lightColor);
    else
        return m_material2->Shade(ray, hit, dirToLight, lightColor);
}

void CheckerBoard::glSetMaterial() const
{
    m_material1->glSetMaterial();
}
} // namespace materials
