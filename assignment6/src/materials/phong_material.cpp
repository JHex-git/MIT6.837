// ====================================================================
// OPTIONAL: 3 pass rendering to fix the specular highlight 
// artifact for small specular exponents (wide specular lobe)
// ====================================================================
#include <GL/gl.h>
// include glCanvas.h to access the preprocessor variable SPECULAR_FIX
#include "opengl/glCanvas.h"  
#include "materials/phong_material.h"
#include "raytrace/ray.h"
#include "raytrace/hit.h"

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
namespace opengl
{
  extern int SPECULAR_FIX_WHICH_PASS;
} // namespace opengl

#endif

// ====================================================================
// Set the OpenGL parameters to render with the given material
// attributes.
// ====================================================================

namespace materials
{
#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
using opengl::SPECULAR_FIX_WHICH_PASS;
#endif

Vec3f PhongMaterial::Shade(const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const
{
    Vec3f h = ray.getDirection();
    h.Negate();
    h += dirToLight;
    h.Normalize();
    
    Vec3f normal = hit.getNormal();
    if (ray.getDirection().Dot3(normal) > 0) normal.Negate(); // self-occlusion when inside object
    float specular_component = std::pow(normal.Dot3(h), m_exponent);
#if SPECULAR_FIX
    specular_component *= std::max(0.0f, normal.Dot3(dirToLight));
#else
    if (normal.Dot3(dirToLight) < 0)
    {
      specular_component = 0;
    }
#endif

    return (std::max(0.0f, normal.Dot3(dirToLight)) * m_diffuseColor + specular_component * m_specularColor) * lightColor;
}

void PhongMaterial::glSetMaterial(void) const {

  GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
  GLfloat specular[4] = {
    m_specularColor.r(),
    m_specularColor.g(),
    m_specularColor.b(),
    1.0};
  GLfloat diffuse[4] = {
    getDiffuseColor().r(),
    getDiffuseColor().g(),
    getDiffuseColor().b(),
    1.0};
  
  // NOTE: GL uses the Blinn Torrance version of Phong...      
  float glexponent = m_exponent;
  if (glexponent < 0) glexponent = 0;
  if (glexponent > 128) glexponent = 128;
  
#if !SPECULAR_FIX 
    
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
  
#else

  // OPTIONAL: 3 pass rendering to fix the specular highlight 
  // artifact for small specular exponents (wide specular lobe)

  if (SPECULAR_FIX_WHICH_PASS == 0) {
    // First pass, draw only the specular highlights
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
    
  } else if (SPECULAR_FIX_WHICH_PASS == 1) {
    // Second pass, compute normal dot light 
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, one);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, zero);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  } else {
    // Third pass, add ambient & diffuse terms
    assert (SPECULAR_FIX_WHICH_PASS == 2);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, zero);
  }

#endif
}
} // namespace materials

