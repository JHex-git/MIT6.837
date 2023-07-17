#include "materials/checker_board.h"

namespace materials
{

Vec3f CheckerBoard::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    assert(false);
    return Vec3f(0, 0, 0);
}

void CheckerBoard::glSetMaterial() const
{
    assert(false);
}
} // namespace materials
