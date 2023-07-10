#include "object3ds/transform.h"

namespace object3ds
{
using utility::Vec3f;
bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    // TODO:
    Vec3f dir = r.getDirection();
    Vec3f orig = r.getOrigin();

    // from world space to object space
    m_matWS2OS.Transform(orig);
    m_matWS2OS.TransformDirection(dir);

    float scale = dir.Length();
    dir.Normalize();
    scale = scale / dir.Length();
    Ray ray(orig, dir);
    if (m_object->intersect(ray, h, tmin) && h.getT() * scale > tmin)
    {
        Vec3f normal = h.getNormal();
        m_mat_normalWS2OS.TransformDirection(normal);
        normal.Normalize();
        h.set(h.getT() / scale, h.getMaterial(), normal, r);
        return true;
    }
    return false;
}

void Transform::paint(void)
{
    assert(false);
}
} // namespace object3ds
