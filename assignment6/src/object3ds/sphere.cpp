#include <GL/gl.h>
#include "object3ds/sphere.h"
#include "object3ds/boundingbox.h"
#include "object3ds/grid.h"
#include "raytrace/raytracing_stats.h"
#include "utility/matrix.h"

namespace object3ds
{
using raytrace::RayTracingStats;

int Sphere::theta_steps = 25;
int Sphere::phi_steps = 25;
bool Sphere::gouraud = false;
constexpr float epsilon = 0.001f;

Sphere::Sphere(const Vec3f &center, float radius, Material* material) : 
    Object3D(material), m_center(center), m_radius(radius) 
{ 
    m_bounding_box = std::make_shared<BoundingBox>(m_center - Vec3f(radius, radius, radius), m_center + Vec3f(radius, radius, radius));
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
    RayTracingStats::IncrementNumIntersections(); 
    bool is_inside = false;
    Vec3f ray_origin = r.getOrigin();
    Vec3f center2ray_origin = ray_origin - m_center;
    is_inside = (center2ray_origin).Length() < m_radius && tmin != std::numeric_limits<float>::lowest();
    float t_p = -center2ray_origin.Dot3(r.getDirection());

    float distance_square = center2ray_origin.Dot3(center2ray_origin) - t_p * t_p;
    float radius_square = m_radius * m_radius;
    if (distance_square > radius_square) // d^2 > r^2
    {
        return false;
    }

    float t_prime = std::sqrt(radius_square - distance_square);
    float t = is_inside ? t_p + t_prime : t_p - t_prime;
    if (t == t_p - t_prime && t < tmin) t = t_p + t_prime; // origin out of sphere but tmin is inside
    if (t < tmin) return false;
    Vec3f normal = ray_origin + t * r.getDirection() - m_center;
    normal.Normalize();
    h.set(t, m_material, normal, r);
    return true;
}   

void Sphere::paint(void) const
{
    m_material->glSetMaterial();
    glBegin(GL_QUADS);
    float phi_step = 180.0 / phi_steps;
    float theta_step = 360.0 / theta_steps;
    for (float iPhi = -90; iPhi < 90; iPhi += phi_step)
    {
        for (float iTheta = 0; iTheta < 360; iTheta += theta_step)
        {
            Vec3f p1 = getCoordinate(iPhi, iTheta);
            Vec3f p2 = getCoordinate(iPhi, std::min(iTheta + theta_step, 360.f));
            Vec3f p3 = getCoordinate(std::min(iPhi + phi_step, 90.f), std::min(iTheta + theta_step, 360.f));
            Vec3f p4 = getCoordinate(std::min(iPhi + phi_step, 90.f), iTheta);
            Vec3f normal;
            if (!gouraud) 
            {
                Vec3f::Cross3(normal, p2 - p1, p4 - p1);
                normal.Normalize();
                glNormal3f(normal[0], normal[1], normal[2]);
                glVertex3f(p1[0], p1[1], p1[2]);
                glVertex3f(p2[0], p2[1], p2[2]);
                glVertex3f(p3[0], p3[1], p3[2]);
                glVertex3f(p4[0], p4[1], p4[2]);
            }
            else 
            {
                normal = p1 - m_center;
                normal.Normalize();
                glNormal3f(normal[0], normal[1], normal[2]);
                glVertex3f(p1[0], p1[1], p1[2]);
                normal = p2 - m_center;
                normal.Normalize();
                glNormal3f(normal[0], normal[1], normal[2]);
                glVertex3f(p2[0], p2[1], p2[2]);
                normal = p3 - m_center;
                normal.Normalize();
                glNormal3f(normal[0], normal[1], normal[2]);
                glVertex3f(p3[0], p3[1], p3[2]);
                normal = p4 - m_center;
                normal.Normalize();
                glNormal3f(normal[0], normal[1], normal[2]);
                glVertex3f(p4[0], p4[1], p4[2]);
            }
        }
    }
    glEnd();
}

void Sphere::insertIntoGrid(Grid *g, Matrix *m)
{
    // Don't need to handle transformations in sphere, fall back to Object3D::insertIntoGrid
    if (m != nullptr)
    {
        Object3D::insertIntoGrid(g,m);
        return;
    }

    float half_diagonal_length = g->getVoxelDiagonalLength() / 2.0;

    // When the bounding box is on the edge of a voxel, the voxel will be missed, so extra epsilon is needed.
    auto min_index = g->getVoxelIndex(m_bounding_box->getMin() - Vec3f(half_diagonal_length, half_diagonal_length, half_diagonal_length));
    for (int i = 0; i < min_index.size(); ++i)
    {
        min_index[i] = std::max(min_index[i], 0);
    }

    // When the bounding box is on the edge of a voxel, the voxel will be missed, so extra voxels are needed.
    auto max_index = g->getVoxelIndex(m_bounding_box->getMax() + Vec3f(half_diagonal_length, half_diagonal_length, half_diagonal_length));
    auto voxel_num = g->getVoxelNum();
    for (int i = 0; i < max_index.size(); ++i)
    {
        max_index[i] = max_index[i] == -1 ? voxel_num[i] - 1 : max_index[i];
    }

    for (int i = min_index[0]; i <= max_index[0]; i++)
    {
        for (int j = min_index[1]; j <= max_index[1]; j++)
        {
            for (int k = min_index[2]; k <= max_index[2]; k++)
            {
                auto voxel_center = g->getVoxelCenter(i, j, k);
                if ((voxel_center - m_center).Length() <= m_radius + half_diagonal_length)
                {
                    // TODO: potential bug: repeated insertion
                    g->addObjectToVoxel(i, j, k, std::shared_ptr<Object3D>(this));
                }
            }
        }
    }
}

Vec3f Sphere::getCoordinate(float phi, float theta) const
{
    return m_center + std::sin(phi * M_PI / 180.0) * m_radius * Vec3f(0, 1, 0) + std::cos(phi * M_PI / 180.0) *(std::cos(theta * M_PI / 180.0) * m_radius * Vec3f(0, 0, 1) + std::sin(theta * M_PI / 180.0) * m_radius * Vec3f(1, 0, 0));
}
} // namespace object3ds
