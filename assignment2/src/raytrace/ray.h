#ifndef _RAY_H
#define _RAY_H

#include <iostream>

#include "utility/vectors.h"

// ====================================================================
// ====================================================================

// Ray class mostly copied from Peter Shirley and Keith Morley

namespace raytrace
{
using utility::Vec3f;
class Ray {

public:
  // CONSTRUCTOR & DESTRUCTOR
  Ray () {}
  Ray (const Vec3f &orig, const Vec3f &dir) {
    origin = orig; 
    direction = dir; }
  Ray (const Ray& r) {*this=r;}

  // ACCESSORS
  const Vec3f& getOrigin() const { return origin; }
  const Vec3f& getDirection() const { return direction; }
  Vec3f pointAtParameter(float t) const {
    return origin+direction*t; }

private:

  // REPRESENTATION
  Vec3f origin;
  Vec3f direction;
};

inline std::ostream &operator<<(std::ostream &os, const Ray &r) {
  os << "Ray <o:" <<r.getOrigin()<<", d:"<<r.getDirection()<<">";
  return os;
} 
} // namespace raytrace


// ====================================================================
// ====================================================================

#endif
