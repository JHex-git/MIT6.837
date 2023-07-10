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
    
    virtual void glInit(int w, int h) = 0;
    virtual void glPlaceCamera(void) = 0;
    virtual void dollyCamera(float dist) = 0;
    virtual void truckCamera(float dx, float dy) = 0;
    virtual void rotateCamera(float rx, float ry) = 0;
};  
} // namespace cameras

#endif