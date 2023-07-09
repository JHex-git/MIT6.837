#ifndef _PERSPECTIVE_CAMERA_H_
#define _PERSPECTIVE_CAMERA_H_
#include "cameras/camera.h"
#include <limits>
#include "utility/vectors.h"

namespace cameras
{
using utility::Vec3f;
using raytrace::Ray;

class PerspectiveCamera : public Camera
{
public:
    // angle is in radian
    PerspectiveCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float angle);

    Ray generateRay(Vec2f point) override;
    float inline getTMin() const override { return 0; }

private:
    Vec3f m_center;
    Vec3f m_direction;
    Vec3f m_up;
    Vec3f m_horizontal;
    float m_radian;
};  
} // namespace cameras


#endif