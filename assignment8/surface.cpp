#include "surface.h"
#include "curve.h"

SurfaceOfRevolution::SurfaceOfRevolution(Curve* c) : Surface(c->getNumVertices())
{
    // for (int i = 0; i < c->getNumVertices(); i++)
    // {
    //     Vec3f v = c->getVertex(i);
    //     float x = v.x();
    //     float y = v.y();
    //     float z = v.z();
    //     float theta = atan2(z, x);
    //     float r = sqrt(x * x + z * z);
    //     Vec3f new_v = Vec3f(r, y, theta);
    //     set(i, new_v);
    // }
}