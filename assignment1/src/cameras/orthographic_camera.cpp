#include "cameras/orthographic_camera.h"
#include "raytrace/ray.h"

namespace cameras
{
using utility::Vec2f;
using utility::Vec3f;

Ray OrthographicCamera::generateRay(Vec2f point)
{
    Vec3f origin = m_center + (point.x() - 0.5) * m_size * m_horizontal + (point.y() - 0.5) * m_size * m_up;
    return Ray(origin, m_direction);
}

OrthographicCamera::OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size)
{
    m_center = center;
    
    m_direction = direction;
    m_direction.Normalize();
    Vec3f::Cross3(m_horizontal, m_direction, up);
    m_horizontal.Normalize();
    Vec3f::Cross3(m_up, m_horizontal, m_direction); // adjust up vector to ensure orthogonality
    m_up.Normalize();

    m_size = size;
}
} // namespace cameras