#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "triangle_mesh.h"
#include <vector>

class ArgParser;

class Spline
{
public:
    Spline() = default;

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) = 0;

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) = 0;
    virtual void OutputBSpline(FILE *file) = 0;

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() const = 0;
    virtual const Vec3f& getVertex(int i) const = 0;
    virtual void set(int i, Vec3f v) = 0;

    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y) = 0;
    virtual void addControlPoint(int selectedPoint, float x, float y) = 0;
    virtual void deleteControlPoint(int selectedPoint) = 0;

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh* OutputTriangles(ArgParser* args) = 0;
};


#endif