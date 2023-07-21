#ifndef _SPLINE_H_
#define _SPLINE_H_

#include "triangle_mesh.h"
#include <vector>

class ArgParser;

class Spline
{
public:
    Spline(int num_vertices) : m_vertices(num_vertices) {}

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) = 0;

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) = 0;
    virtual void OutputBSpline(FILE *file) = 0;

    // FOR CONTROL POINT PICKING
    int getNumVertices() { return m_vertices.size(); }
    Vec3f getVertex(int i) { return m_vertices[i]; }
    void set(int i, Vec3f v) { m_vertices[i] = v; }

    // FOR EDITING OPERATIONS
    virtual void moveControlPoint(int selectedPoint, float x, float y) { assert(false); }
    virtual void addControlPoint(int selectedPoint, float x, float y) { assert(false); }
    virtual void deleteControlPoint(int selectedPoint) { assert(false); }

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh* OutputTriangles(ArgParser* args) { assert(false); return nullptr; }

protected:
    std::vector<Vec3f> m_vertices;
};


#endif