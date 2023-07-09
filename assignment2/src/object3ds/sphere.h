#ifndef _SPHERE_H_
#define _SPHERE_H_
#include "object3ds/object3d.h"

namespace object3ds
{
using raytrace::Ray;
using raytrace::Hit;
using utility::Vec3f;

class Sphere : public Object3D
{
public:
    Sphere(const Vec3f &center, float radius, Material* material) : 
        Object3D(material), m_center(center), m_radius(radius) { }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

private:
    Vec3f m_center;
    float m_radius;
};  
} // namespace object3ds

#endif