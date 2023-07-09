#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_
#include "object3ds/object3d.h"
#include "utility/matrix.h"

namespace object3ds
{
using raytrace::Ray;
using raytrace::Hit;
using utility::Matrix;

class Transform : public Object3D
{
public:
    Transform(const Matrix &mat, Object3D* object) : m_mat(mat), m_object(object) { }

    bool intersect(const Ray &r, Hit &h, float tmin) override;

private:
    Matrix m_mat;
    Object3D* m_object;
};  
} // namespace object3ds

#endif