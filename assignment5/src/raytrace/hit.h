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
  Hit(float _t, Material *m, Vec3f n) { 
    t = _t; material = m; normal = n; }
  Hit(const Hit &h) { 
    t = h.t; 
    material = h.material; 
    normal = h.normal; 
    intersectionPoint = h.intersectionPoint; }
  ~Hit() {}

  // ACCESSORS
  float getT() const { return t; }
  Material* getMaterial() const { return material; }
  Vec3f getNormal() const { return normal; }
  Vec3f getIntersectionPoint() const { return intersectionPoint; }
  
  // MODIFIER
  void set(float _t, Material *m, Vec3f n, const Ray &ray) {
    t = _t; material = m; normal = n; 
    intersectionPoint = ray.pointAtParameter(t); }

private: 

  // REPRESENTATION
  float t;
  Material *material;
  Vec3f normal;
  Vec3f intersectionPoint;

};

inline std::ostream &operator<<(std::ostream &os, const Hit &h) {
  os << "Hit <" <<h.getT()<<", "<<h.getNormal()<<">";
  return os;
}
// ====================================================================
// ====================================================================
  
} // namespace raytrace


#endif
