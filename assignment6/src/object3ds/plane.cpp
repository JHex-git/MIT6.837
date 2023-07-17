#include "object3ds/plane.h"
#include "raytrace/raytracing_stats.h"
#include <GL/gl.h>

namespace object3ds
{
using raytrace::RayTracingStats;

bool Plane::intersect(const Ray &r, Hit &h, float tmin)
{
    RayTracingStats::IncrementNumIntersections(); 
    // if plane parallel to ray
    if (std::abs(r.getDirection().Dot3(m_normal)) < 1e-6) return false;

    float t = (m_offset - r.getOrigin().Dot3(m_normal)) / r.getDirection().Dot3(m_normal);

    if (t < tmin) return false;
    h.set(t, m_material, m_normal, r);
    return true;
}

void Plane::paint(void) const
{
    m_material->glSetMaterial();
    float t = m_offset / m_normal.Dot3(m_normal);
    Vec3f p = t * m_normal; // project origin to plane
    Vec3f v(1, 0, 0);
    if (m_normal.y() == 0 && m_normal.z() == 0) v = Vec3f(0, 1, 0);
    Vec3f b1;
    Vec3f::Cross3(b1, v, m_normal);
    b1.Normalize();
    Vec3f b2;
    Vec3f::Cross3(b2, m_normal, b1);
    b2.Normalize();
    
    glBegin(GL_QUADS);
    glNormal3f(m_normal[0], m_normal[1], m_normal[2]);
    glVertex3f(p[0] + b1[0] * 10000 + b2[0] * 10000, p[1] + b1[1] * 10000 + b2[1] * 10000, p[2] + b1[2] * 10000 + b2[2] * 10000);
    glVertex3f(p[0] + b1[0] * 10000 - b2[0] * 10000, p[1] + b1[1] * 10000 - b2[1] * 10000, p[2] + b1[2] * 10000 - b2[2] * 10000);
    glVertex3f(p[0] - b1[0] * 10000 - b2[0] * 10000, p[1] - b1[1] * 10000 - b2[1] * 10000, p[2] - b1[2] * 10000 - b2[2] * 10000);
    glVertex3f(p[0] - b1[0] * 10000 + b2[0] * 10000, p[1] - b1[1] * 10000 + b2[1] * 10000, p[2] - b1[2] * 10000 + b2[2] * 10000);
    glEnd();
}
} // namespace object3ds
