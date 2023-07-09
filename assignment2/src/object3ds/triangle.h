#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_
#include "object3ds/object3d.h"

namespace object3ds
{
using raytrace::Ray;
using raytrace::Hit;
using utility::Vec3f;

class Triangle : public Object3D
{
public:
    Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material* material) : 
        Object3D(material), m_point_a(a), m_point_b(b), m_point_c(c) { }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

private:
    Vec3f m_point_a;
    Vec3f m_point_b;
    Vec3f m_point_c;
};  
} // namespace object3ds

#endif