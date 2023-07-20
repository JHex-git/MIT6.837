#include <GL/gl.h>
#include <GL/glu.h>
#include "cameras/perspective_camera.h"
#include "raytrace/ray.h"
#include "utility/matrix.h"

namespace cameras
{
using utility::Matrix;

PerspectiveCamera::PerspectiveCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float angle)
    : m_center(center), m_direction(direction), m_up(up), m_radian(angle)
{
    m_direction.Normalize();
    Vec3f::Cross3(m_horizontal, m_direction, m_up);
    m_horizontal.Normalize();
    Vec3f::Cross3(m_up, m_horizontal, m_direction);
    m_up.Normalize();
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
    Vec3f direction = m_direction * (0.5 / std::tan(m_radian / 2)) + m_horizontal * (point.x() - 0.5) + m_up * (point.y() - 0.5);
    direction.Normalize();
    return Ray(m_center, direction);
}

void PerspectiveCamera::glInit(int w, int h)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(m_radian*180.0/3.14159, (float)w/float(h), 0.5, 40.0);
}

void PerspectiveCamera::glPlaceCamera(void)
{
  gluLookAt(m_center.x(), m_center.y(), m_center.z(),
            m_center.x()+m_direction.x(), m_center.y()+m_direction.y(), m_center.z()+m_direction.z(),
            m_up.x(), m_up.y(), m_up.z());
}

// ====================================================================
// dollyCamera, truckCamera, and RotateCamera
//
// Asumptions:
//  - up is really up (i.e., it hasn't been changed
//    to point to "screen up")
//  - up and direction are normalized
// Special considerations:
//  - If your constructor precomputes any vectors for
//    use in 'generateRay', you will likely to recompute those
//    values at athe end of the these three routines
// ====================================================================

// ====================================================================
// dollyCamera: Move camera along the direction vector
// ====================================================================

void PerspectiveCamera::dollyCamera(float dist)
{
  m_center += m_direction * dist;
}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void PerspectiveCamera::truckCamera(float dx, float dy)
{
  m_center += m_horizontal * dx + m_up * dy;
}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
  // Don't let the model flip upside-down (There is a singularity
  // at the poles when 'up' and 'direction' are aligned)
  float tiltAngle = acos(m_up.Dot3(m_direction));
  if (tiltAngle-ry > 3.13)
    ry = tiltAngle - 3.13;
  else if (tiltAngle-ry < 0.01)
    ry = tiltAngle - 0.01;

  Matrix rotMat = Matrix::MakeAxisRotation(m_up, rx);
  rotMat *= Matrix::MakeAxisRotation(m_horizontal, ry);

  rotMat.Transform(m_center);
  rotMat.TransformDirection(m_direction);
  m_direction.Normalize();
  rotMat.TransformDirection(m_horizontal);
  m_horizontal.Normalize();
  rotMat.TransformDirection(m_up);
  m_up.Normalize();
}
} // namespace cameras