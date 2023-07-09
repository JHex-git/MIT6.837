#ifndef _CAMERA_H_
#define _CAMERA_H_

namespace utility
{
    class Vec2f;
} // namespace utility

namespace raytrace
{
    class Ray;
} // namespace raytrace

namespace cameras
{
using utility::Vec2f;
using raytrace::Ray;
class Camera
{
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};  
} // namespace cameras

#endif