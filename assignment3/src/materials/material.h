#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "utility/vectors.h"

// ====================================================================
// ====================================================================
namespace raytrace
{
  class Ray;
  class Hit;
} // namespace raytrace

// You will extend this class in later assignments
namespace materials
{
using utility::Vec3f;
using raytrace::Ray;
using raytrace::Hit;

class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }

  virtual Vec3f Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const = 0;

  virtual void glSetMaterial() const = 0;

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
};
  
} // namespace materials


// ====================================================================
// ====================================================================

#endif
