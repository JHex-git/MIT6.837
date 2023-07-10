#include <GL/gl.h>
#include <GL/glu.h>
#include "cameras/orthographic_camera.h"
#include "raytrace/ray.h"
#include "utility/matrix.h"

namespace cameras
{
using utility::Vec2f;
using utility::Vec3f;
using utility::Matrix;

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

void OrthographicCamera::glInit(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (w > h)
        glOrtho(-m_size/2.0, m_size/2.0, -m_size*(float)h/(float)w/2.0, m_size*(float)h/(float)w/2.0, 0.5, 40.0);
    else
        glOrtho(-m_size*(float)w/(float)h/2.0, m_size*(float)w/(float)h/2.0, -m_size/2.0, m_size/2.0, 0.5, 40.0);
}

void OrthographicCamera::glPlaceCamera(void)
{
    gluLookAt(m_center.x(), m_center.y(), m_center.z(),
                m_center.x() + m_direction.x(), m_center.y() + m_direction.y(), m_center.z() + m_direction.z(),
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

void OrthographicCamera::dollyCamera(float dist)
{
    m_center += m_direction * dist;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================


}

// ====================================================================
// truckCamera: Translate camera perpendicular to the direction vector
// ====================================================================

void OrthographicCamera::truckCamera(float dx, float dy)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, m_direction, m_up);
    horizontal.Normalize();

    Vec3f screenUp;
    Vec3f::Cross3(screenUp, horizontal, m_direction);

    m_center += horizontal*dx + screenUp*dy;

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================


}

// ====================================================================
// rotateCamera: Rotate around the up and horizontal vectors
// ====================================================================

void OrthographicCamera::rotateCamera(float rx, float ry)
{
    Vec3f horizontal;
    Vec3f::Cross3(horizontal, m_direction, m_up);
    horizontal.Normalize();

    // Don't let the model flip upside-down (There is a singularity
    // at the poles when 'up' and 'direction' are aligned)
    float tiltAngle = acos(m_up.Dot3(m_direction));
    if (tiltAngle-ry > 3.13)
        ry = tiltAngle - 3.13;
    else if (tiltAngle-ry < 0.01)
        ry = tiltAngle - 0.01;

    Matrix rotMat = Matrix::MakeAxisRotation(m_up, rx);
    rotMat *= Matrix::MakeAxisRotation(horizontal, ry);

    rotMat.Transform(m_center);
    rotMat.TransformDirection(m_direction);

    // ===========================================
    // ASSIGNMENT 3: Fix any other affected values
    // ===========================================


}
} // namespace cameras