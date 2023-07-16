#ifndef _OBJECT3D_H_
#define _OBJECT3D_H_
#include "raytrace/ray.h"
#include "raytrace/hit.h"
#include "materials/material.h"
#include <memory>

namespace utility
{
    class Matrix;
}

namespace object3ds
{
class BoundingBox;
class Grid;

using raytrace::Ray;
using raytrace::Hit;
using materials::Material;
using utility::Matrix;

class Object3D
{
public:
    Object3D() : m_material(nullptr) {}
    Object3D(Material* material) : m_material(material) {}
    virtual ~Object3D() = default;

    virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
    virtual void paint(void) const = 0;
    virtual void insertIntoGrid(Grid *g, Matrix *m);

    std::shared_ptr<BoundingBox> getBoundingBox() const { return m_bounding_box; }

protected:
    Material* m_material;
    std::shared_ptr<BoundingBox> m_bounding_box;
};  
} // namespace object3ds


#endif