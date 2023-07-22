#ifndef _CURVE_H_
#define _CURVE_H_

#include "spline.h"
#include "matrix.h"
#include <vector>

class Curve : public Spline
{
public:
  Curve(int num_vertices) : m_vertices(num_vertices) {}

  // FOR VISUALIZATION
  virtual void Paint(ArgParser *args) override;

  // FOR CONVERTING BETWEEN SPLINE TYPES
  virtual void OutputBezier(FILE *file) override = 0;
  virtual void OutputBSpline(FILE *file) override = 0;

  // FOR CONTROL POINT PICKING
  int getNumVertices() const override { return m_vertices.size(); }
  const Vec3f& getVertex(int i) const override { return m_vertices[i]; }
  void set(int i, Vec3f v) override { m_vertices[i] = v; }

  // // FOR EDITING OPERATIONS
  void moveControlPoint(int selectedPoint, float x, float y) override
  {
    m_vertices[selectedPoint].Set(x, y, 0);
  }
  virtual void addControlPoint(int selectedPoint, float x, float y) override = 0;
  virtual void deleteControlPoint(int selectedPoint) override = 0;

  // FOR GENERATING TRIANGLES
  TriangleMesh* OutputTriangles(ArgParser* args) override { return nullptr; }

  virtual Vec3f getCurvePointAtParam(float t) const = 0;
  int getTessellatedCurvePointsNum(int tessellation) { return tessellation * getControlPointsWindowNum(); }

protected:
  virtual int getControlPointsWindowNum() const = 0;

  std::vector<Vec3f> m_vertices;
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

  Vec3f getCurvePointAtParam(float t) const override;
protected:
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

  Vec3f getCurvePointAtParam(float t) const override;
protected:
  int getControlPointsWindowNum() const override;

private:
  static float B_bspline_data[16];
  static Matrix B_bspline;
};
#endif