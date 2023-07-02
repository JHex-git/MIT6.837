#ifndef _OTTHOGRAPHIC_CAMERA_H_
#define _OTTHOGRAPHIC_CAMERA_H_
#include "camera.h"

namespace cameras
{
using utility::Vec3f;
using raytrace::Ray;

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(const Vec3f &center, const Vec3f &direction, const Vec3f &up, float size);

    Ray generateRay(Vec2f point) override;
    float getTMin() const override;
};  
} // namespace cameras


#endif