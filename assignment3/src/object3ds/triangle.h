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
        Object3D(material), m_point_a(a), m_point_b(b), m_point_c(c) 
    {
        Vec3f::Cross3(m_normal, b - a, c - a);
        m_normal.Normalize();
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint(void) override;

private:
    float determinant(const float mat[9]) const;

    Vec3f m_point_a;
    Vec3f m_point_b;
    Vec3f m_point_c;
    Vec3f m_normal;
};  
} // namespace object3ds

#endif