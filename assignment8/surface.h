#ifndef _SURFACE_H_
#define _SURFACE_H_

#include "curve.h"

class Surface : public Spline
{
public:
    Surface(int num_vertices) : Spline(num_vertices) {}

    // FOR VISUALIZATION
    virtual void Paint(ArgParser *args) override { assert(false); }

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) override = 0;
    virtual void OutputBSpline(FILE *file) override = 0;
};

class SurfaceOfRevolution : public Surface
{
public:
    SurfaceOfRevolution(Curve* c);

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) override { assert(false); }
    virtual void OutputBSpline(FILE *file) override { assert(false); }
};

class BezierPatch : public Surface
{
public:
    BezierPatch() : Surface(0) {}

    // FOR CONVERTING BETWEEN SPLINE TYPES
    virtual void OutputBezier(FILE *file) override { assert(false); }
    virtual void OutputBSpline(FILE *file) override { assert(false); }
};
#endif