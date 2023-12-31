#ifndef _HIT_H
#define _HIT_H

#include "utility/vectors.h"
#include "raytrace/ray.h"

namespace materials
{
  class Material;
} // namespace materials


// ====================================================================
// ====================================================================
namespace raytrace
{
using utility::Vec3f;
using materials::Material;

class Hit {
public:

  // CONSTRUCTOR & DESTRUCTOR
  Hit() { material = NULL; }
  Hit(float _t, Material *m) { 
    t = _t; material = m; }
  Hit(const Hit &h) { 
    t = h.t; 
    material = h.material; 
    intersectionPoint = h.intersectionPoint; }
  ~Hit() {}

  // ACCESSORS
  float getT() const { return t; }
  Material* getMaterial() const { return material; }
  Vec3f getIntersectionPoint() const { return intersectionPoint; }
  
  // MODIFIER
  void set(float _t, Material *m, const Ray &ray) {
    t = _t; material = m; 
    intersectionPoint = ray.pointAtParameter(t); }

private: 

  // REPRESENTATION
  float t;
  Material *material;
  Vec3f intersectionPoint;

};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
  os << "Hit <t:" << h.getT() <<">";
  return os;
}
// ====================================================================
// ====================================================================
  
} // namespace raytrace


#endif
