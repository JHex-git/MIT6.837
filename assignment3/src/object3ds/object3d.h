#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_
#include "raytrace/ray.h"
#include "raytrace/hit.h"
#include "materials/material.h"

namespace object3ds
{
using raytrace::Ray;
using raytrace::Hit;
using materials::Material;

class Object3D
{
public:
    Object3D() : m_material(nullptr) {}
    Object3D(Material* material) : m_material(material) {}

    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint(void) = 0;

protected:
    Material* m_material;
};  
} // namespace object3ds


#endif