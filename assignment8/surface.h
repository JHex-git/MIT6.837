#ifndef _SURFACE_H_
#define _SURFACE_H_

#include "curve.h"
#include <array>

class Surface : public Spline
{
public:
    Surface() = default;

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) override = 0;

    // FOR CONTROL POINT PICKING
    virtual int getNumVertices() const = 0;
    virtual const Vec3f& getVertex(int i) const = 0;
    virtual void set(int i, Vec3f v) = 0;

    // FOR EDITING OPERATIONS
    void moveControlPoint(int selectedPoint, float x, float y) override = 0;
    void addControlPoint(int selectedPoint, float x, float y) override = 0;
    void deleteControlPoint(int selectedPoint) override = 0;

    virtual TriangleMesh* OutputTriangles(ArgParser* args) override = 0;
    virtual void OutputBezier(FILE *file) override = 0;
    virtual void OutputBSpline(FILE *file) override = 0;
};

class SurfaceOfRevolution : public Surface
{
public:
    SurfaceOfRevolution(Curve* c);

    void Paint(ArgParser *args) override;

    // FOR CONTROL POINT PICKING
    int getNumVertices() const override { return m_curve->getNumVertices(); }
    const Vec3f& getVertex(int i) const override { return m_curve->getVertex(i); }
    void set(int i, Vec3f v) override { m_curve->set(i, v); }

    // FOR EDITING OPERATIONS
    void moveControlPoint(int selectedPoint, float x, float y) { m_curve->moveControlPoint(selectedPoint, x, y); }
    void addControlPoint(int selectedPoint, float x, float y) override { m_curve->addControlPoint(selectedPoint, x, y); }
    void deleteControlPoint(int selectedPoint) override { m_curve->deleteControlPoint(selectedPoint); }

    // FOR CONVERTING BETWEEN SPLINE TYPES
    void OutputBezier(FILE *file) override;
    void OutputBSpline(FILE *file) override;
    TriangleMesh* OutputTriangles(ArgParser* args) override;
private:
    Curve* m_curve;
};

class BezierPatch : public Surface
{
public:
    BezierPatch() = default;

    void Paint(ArgParser *args) override;

    // FOR CONTROL POINT PICKING
    int getNumVertices() const { return m_vertices.size(); }
    const Vec3f& getVertex(int i) const { return m_vertices[i]; }
    void set(int i, Vec3f v) { m_vertices[i] = v; }

    // FOR EDITING OPERATIONS
    void moveControlPoint(int selectedPoint, float x, float y) override {}
    void addControlPoint(int selectedPoint, float x, float y) override {}
    void deleteControlPoint(int selectedPoint) override {}

    // FOR CONVERTING BETWEEN SPLINE TYPES
    void OutputBezier(FILE *file) override {}
    void OutputBSpline(FILE *file) override {}
    TriangleMesh* OutputTriangles(ArgParser* args) override;

private:
    Vec3f getSurfacePointAtParam(float t, float s) const;
    Vec3f CurveBezier(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, float alpha) const;

    std::array<Vec3f, 16> m_vertices;
};
#endif