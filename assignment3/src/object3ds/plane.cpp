#include "object3ds/plane.h"

namespace object3ds
{
bool Plane::intersect(const Ray &r, Hit &h, float tmin)
{
    // if plane parallel to ray
    if (std::abs(r.getDirection().Dot3(m_normal)) < 1e-6) return false;

    float t = (m_offset - r.getOrigin().Dot3(m_normal)) / r.getDirection().Dot3(m_normal);

    if (t < tmin) return false;
    h.set(t, m_material, m_normal, r);
    return true;
}

void Plane::paint(void)
{
    assert(false);
}
} // namespace object3ds
