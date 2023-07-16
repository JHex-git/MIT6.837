#include "object3ds/transform.h"
#include "object3ds/boundingbox.h"
#include <GL/gl.h>

namespace object3ds
{
using utility::Vec3f;


Transform::Transform(const Matrix &mat, Object3D* object) : m_matTransform(mat), m_matWS2OS(mat), m_object(object) 
{
    Vec3f min = m_object->getBoundingBox()->getMin();
    Vec3f max = m_object->getBoundingBox()->getMax();
    Vec3f v[8];
    // transform eight corners of bounding box
    v[0] = Vec3f(min.x(), min.y(), min.z());
    v[1] = Vec3f(min.x(), min.y(), max.z());
    v[2] = Vec3f(min.x(), max.y(), min.z());
    v[3] = Vec3f(min.x(), max.y(), max.z());
    v[4] = Vec3f(max.x(), min.y(), min.z());
    v[5] = Vec3f(max.x(), min.y(), max.z());
    v[6] = Vec3f(max.x(), max.y(), min.z());
    v[7] = Vec3f(max.x(), max.y(), max.z());
    for (int i = 0; i < 8; i++)
    {
        m_matTransform.Transform(v[i]);
    }
    // get axis-aligned bounding box
    min = max = v[0];
    for (int i = 1; i < 8; i++)
    {
        min = Vec3f(min2(min.x(), v[i].x()),
            min2(min.y(), v[i].y()),
            min2(min.z(), v[i].z()));
        max = Vec3f(max2(max.x(), v[i].x()),
            max2(max.y(), v[i].y()),
            max2(max.z(), v[i].z()));
    }
    m_bounding_box = std::make_shared<BoundingBox>(min, max);

    m_matWS2OS.Inverse();
    m_mat_normalWS2OS = m_matWS2OS;
    m_mat_normalWS2OS.Transpose();
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
    Vec3f dir = r.getDirection();
    Vec3f orig = r.getOrigin();

    // from world space to object space
    m_matWS2OS.Transform(orig);
    m_matWS2OS.TransformDirection(dir);

    float scale = dir.Length();
    dir.Normalize();
    scale = scale / dir.Length();
    Ray ray(orig, dir);
    if (m_object->intersect(ray, h, tmin) && h.getT() * scale > tmin)
    {
        Vec3f normal = h.getNormal();
        m_mat_normalWS2OS.TransformDirection(normal);
        normal.Normalize();
        h.set(h.getT() / scale, h.getMaterial(), normal, r);
        return true;
    }
    return false;
}

void Transform::paint(void) const
{
    glPushMatrix();
    GLfloat *glMatrix = m_matTransform.glGet();
    glMultMatrixf(glMatrix);
    delete[] glMatrix;
    m_object->paint();
    glPopMatrix();
}

void Transform::insertIntoGrid(Grid *g, Matrix *m)
{
    Matrix mat = m_matTransform;
    if (m != nullptr) mat = (*m) * mat;
    m_object->insertIntoGrid(g, &mat);
}
} // namespace object3ds
