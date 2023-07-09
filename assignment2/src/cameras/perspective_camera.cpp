#include "cameras/perspective_camera.h"
#include "raytrace/ray.h"

namespace cameras
{
PerspectiveCamera::PerspectiveCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float angle)
    : m_center(center), m_direction(direction), m_up(up), m_angle(angle)
{
    m_direction.Normalize();
    Vec3f::Cross3(m_horizontal, m_direction, m_up);
    m_horizontal.Normalize();
    Vec3f::Cross3(m_up, m_horizontal, m_direction);
    m_up.Normalize();
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    Vec3f direction = m_direction * (1.0 / std::tan(m_angle)) + m_horizontal * (point.x() - 0.5) + m_up * (point.y() - 0.5);
    direction.Normalize();
    return Ray(m_center, direction);
}
} // namespace cameras