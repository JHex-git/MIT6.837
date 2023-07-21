#ifndef _OTTHOGRAPHIC_CAMERA_H_
#define _OTTHOGRAPHIC_CAMERA_H_
#include "cameras/camera.h"
#include <limits>
#include "utility/vectors.h"

namespace cameras
{
using utility::Vec3f;
using raytrace::Ray;

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size);

    Ray generateRay(Vec2f point) override;
    float inline getTMin() const override { return std::numeric_limits<float>::lowest(); }

    void glInit(int w, int h) override;
    void glPlaceCamera(void) override;
    void dollyCamera(float dist) override;
    void truckCamera(float dx, float dy) override;
    void rotateCamera(float rx, float ry) override;

private:
    Vec3f m_center;
    Vec3f m_direction;
    Vec3f m_up;
    Vec3f m_horizontal;
    float m_size;
};  
} // namespace cameras


#endif