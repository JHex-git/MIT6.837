#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "raytrace/ray.h"

namespace cameras
{
using utility::Vec2f;
class Camera
{
public:
    virtual Ray generateRay(Vec2f point) = 0;
    virtual float getTMin() const = 0;
};  
} // namespace cameras

#endif