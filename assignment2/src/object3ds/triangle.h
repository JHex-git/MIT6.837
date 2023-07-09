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
        float edge_a = (c - b).Length();
        float edge_b = (a - c).Length();
        float edge_c = (b - a).Length();
        float p = (edge_a + edge_b + edge_c) / 2;
        m_area = sqrt(p * (p - edge_a) * (p - edge_b) * (p - edge_c));
        std::cout << "area: " << m_area << std::endl;
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

private:
    float determinant(const float mat[9]) const;

    Vec3f m_point_a;
    Vec3f m_point_b;
    Vec3f m_point_c;
    float m_area;
    Vec3f m_normal;
};  
} // namespace object3ds

#endif