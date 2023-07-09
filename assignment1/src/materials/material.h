#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "utility/vectors.h"

// ====================================================================
// ====================================================================

// You will extend this class in later assignments
namespace materials
{
using utility::Vec3f; 
class Material {

public:

  // CONSTRUCTORS & DESTRUCTOR
  Material(const Vec3f &d_color) { diffuseColor = d_color; }
  virtual ~Material() {}

  // ACCESSORS
  virtual Vec3f getDiffuseColor() const { return diffuseColor; }

protected:

  // REPRESENTATION
  Vec3f diffuseColor;
};
  
} // namespace materials


// ====================================================================
// ====================================================================

#endif
