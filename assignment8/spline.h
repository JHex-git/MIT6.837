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
    virtual void OutputBezier(FILE *file) {}
    virtual void OutputBSpline(FILE *file) {}

    // FOR CONTROL POINT PICKING
    int getNumVertices() const { return m_vertices.size(); }
    const Vec3f& getVertex(int i) const { return m_vertices[i]; }
    void set(int i, Vec3f v) { m_vertices[i] = v; }

    // FOR EDITING OPERATIONS
    void moveControlPoint(int selectedPoint, float x, float y)
    {
        m_vertices[selectedPoint].Set(x, y, 0);
    }
    virtual void addControlPoint(int selectedPoint, float x, float y) {}
    virtual void deleteControlPoint(int selectedPoint) {}

    // FOR GENERATING TRIANGLES
    virtual TriangleMesh* OutputTriangles(ArgParser* args) { return nullptr; }

protected:
    std::vector<Vec3f> m_vertices;
};


#endif