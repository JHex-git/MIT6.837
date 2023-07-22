#ifndef _SURFACE_H_
#define _SURFACE_H_

#include "curve.h"

class Surface : public Spline
{
public:
    Surface(int num_vertices) : Spline(num_vertices) {}

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) override {}

    virtual Vec3f getCurvePointAtParam(float s, float t) const = 0;

    virtual TriangleMesh* OutputTriangles(ArgParser* args) override {}
};

class SurfaceOfRevolution : public Surface
{
public:
    SurfaceOfRevolution(Curve* c);

    void Paint(ArgParser *args) override;

    Vec3f getCurvePointAtParam(float s, float t) const override { assert(false); }

    TriangleMesh* OutputTriangles(ArgParser* args) override;

private:
    Curve* m_curve;
};

class BezierPatch : public Surface
{
public:
    BezierPatch() : Surface(0) {}

    Vec3f getCurvePointAtParam(float s, float t) const override;

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) override { assert(false); }
    virtual void OutputBSpline(FILE *file) override { assert(false); }

private:
    Vec3f CurveBezier(const Vec3f& p0, const Vec3f& p1, const Vec3f& p2, const Vec3f& p3, float alpha) const;
};
#endif