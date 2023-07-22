#ifndef _CURVE_H_
#define _CURVE_H_

#include "spline.h"
#include "matrix.h"
#include <vector>

class Curve : public Spline
{
public:
  Curve(int num_vertices) : Spline(num_vertices) {}

  // FOR VISUALIZATION
  virtual void Paint(ArgParser *args) override;

  // FOR CONVERTING BETWEEN SPLINE TYPES
  virtual void OutputBezier(FILE *file) override = 0;
  virtual void OutputBSpline(FILE *file) override = 0;

  // // FOR EDITING OPERATIONS
  virtual void addControlPoint(int selectedPoint, float x, float y) override = 0;
  virtual void deleteControlPoint(int selectedPoint) override = 0;

  // // FOR GENERATING TRIANGLES
  // virtual TriangleMesh* OutputTriangles(ArgParser* args) override { assert(false); return nullptr; }

protected:
  virtual Vec3f getCurvePointAtParam(float t) const = 0;
  virtual int getControlPointsWindowNum() const = 0;

};

class BezierCurve : public Curve
{
public:
  BezierCurve(int num_vertices) : Curve(num_vertices) {}

  void OutputBezier(FILE *file) override;
  void OutputBSpline(FILE *file) override;

  // // FOR EDITING OPERATIONS
  void addControlPoint(int selectedPoint, float x, float y) override;
  void deleteControlPoint(int selectedPoint) override;
  
  static const Matrix& GetB() { return B_bezier; }

protected:
  Vec3f getCurvePointAtParam(float t) const override;
  int getControlPointsWindowNum() const override;;
  
private:
  static float B_bezier_data[16];
  static Matrix B_bezier;
};

class BSplineCurve : public Curve
{
public:
  BSplineCurve(int num_vertices) : Curve(num_vertices) {}

  void OutputBezier(FILE *file) override;
  void OutputBSpline(FILE *file) override;

  static const Matrix& GetB() { return B_bspline; }

  void addControlPoint(int selectedPoint, float x, float y) override;
  void deleteControlPoint(int selectedPoint) override;

protected:
  Vec3f getCurvePointAtParam(float t) const override;
  int getControlPointsWindowNum() const override;

private:
  static float B_bspline_data[16];
  static Matrix B_bspline;
};
#endif