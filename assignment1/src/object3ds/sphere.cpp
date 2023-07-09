#include "object3ds/sphere.h"

namespace object3ds
{
bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
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
    if (t < tmin) return false;
    h.set(t, m_material, r);
    return true;
}   
} // namespace object3ds
