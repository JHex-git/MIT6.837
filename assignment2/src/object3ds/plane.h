#ifndef _PLANE_H_
#define _PLANE_H_
#include "object3ds/object3d.h"

namespace object3ds
{
using raytrace::Ray;
using raytrace::Hit;
using utility::Vec3f;

class Plane : public Object3D
{
public:
    // normal point to the direction that away from origin
    Plane(const Vec3f &normal, float d, Material* material) : Object3D(material), m_normal(normal), m_offset(d)
    {
        m_normal.Normalize();
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

private:
    Vec3f m_normal;
    float m_offset;
};  
} // namespace object3ds

#endif