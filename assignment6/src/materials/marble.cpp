#include "materials/marble.h"

namespace materials
{

Marble::Marble(Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude) : Material(Vec3f(0, 0, 0))
{
    assert(false);
}

Vec3f Marble::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    assert(false);
    return Vec3f(0, 0, 0);
}

void Marble::glSetMaterial() const
{
    assert(false);
}
}