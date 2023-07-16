#include <GL/gl.h>
#include "object3ds/triangle.h"
#include "object3ds/boundingbox.h"
#include "object3ds/grid.h"

namespace object3ds
{

Triangle::Triangle(const Vec3f &a, const Vec3f &b, const Vec3f &c, Material* material) : 
    Object3D(material), m_point_a(a), m_point_b(b), m_point_c(c) 
{
    Vec3f::Cross3(m_normal, b - a, c - a);
    m_normal.Normalize();
    Vec3f min(std::min(a.x(), std::min(b.x(), c.x())), std::min(a.y(), std::min(b.y(), c.y())), std::min(a.z(), std::min(b.z(), c.z())));
    Vec3f max(std::max(a.x(), std::max(b.x(), c.x())), std::max(a.y(), std::max(b.y(), c.y())), std::max(a.z(), std::max(b.z(), c.z())));
    m_bounding_box = std::make_shared<BoundingBox>(min, max);
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
    // Use Baricentric Coordinates to determine if the ray intersects the triangle
    // Use Cramer's Rule to solve for beta and gamma
    Vec3f Ro = r.getOrigin();
    Vec3f Rd = r.getDirection();
    float A_mat[9] = {
        m_point_a.x() - m_point_b.x(), m_point_a.x() - m_point_c.x(), Rd.x(),
        m_point_a.y() - m_point_b.y(), m_point_a.y() - m_point_c.y(), Rd.y(),
        m_point_a.z() - m_point_b.z(), m_point_a.z() - m_point_c.z(), Rd.z()
    };
    float A = determinant(A_mat);

    float t_mat[9] = {
        m_point_a.x() - m_point_b.x(), m_point_a.x() - m_point_c.x(), m_point_a.x() - Ro.x(),
        m_point_a.y() - m_point_b.y(), m_point_a.y() - m_point_c.y(), m_point_a.y() - Ro.y(),
        m_point_a.z() - m_point_b.z(), m_point_a.z() - m_point_c.z(), m_point_a.z() - Ro.z()
    };
    float t = determinant(t_mat) / A;
    if (t < tmin) return false;

    float beta_mat[9] = {
        m_point_a.x() - Ro.x(), m_point_a.x() - m_point_c.x(), Rd.x(),
        m_point_a.y() - Ro.y(), m_point_a.y() - m_point_c.y(), Rd.y(),
        m_point_a.z() - Ro.z(), m_point_a.z() - m_point_c.z(), Rd.z()
    };
    float beta = determinant(beta_mat) / A;
    if (beta < 0 || beta > 1) return false;

    float gamma_mat[9] = {
        m_point_a.x() - m_point_b.x(), m_point_a.x() - Ro.x(), Rd.x(),
        m_point_a.y() - m_point_b.y(), m_point_a.y() - Ro.y(), Rd.y(),
        m_point_a.z() - m_point_b.z(), m_point_a.z() - Ro.z(), Rd.z()
    };
    float gamma = determinant(gamma_mat) / A;
    if (gamma < 0|| beta + gamma > 1)
        return false;
    
    h.set(t, m_material, m_normal, r);
    return true;
}

float Triangle::determinant(const float mat[9]) const
{
    return mat[0] * mat[4] * mat[8] + mat[1] * mat[5] * mat[6] + mat[2] * mat[3] * mat[7] - mat[2] * mat[4] * mat[6] - mat[1] * mat[3] * mat[8] - mat[0] * mat[5] * mat[7];
}

void Triangle::paint(void) const
{
    m_material->glSetMaterial();
    glBegin(GL_TRIANGLES);
    glNormal3f(m_normal.x(), m_normal.y(), m_normal.z());
    glVertex3f(m_point_a.x(), m_point_a.y(), m_point_a.z());
    glVertex3f(m_point_b.x(), m_point_b.y(), m_point_b.z());
    glVertex3f(m_point_c.x(), m_point_c.y(), m_point_c.z());
    glEnd();
}

} // namespace object3ds