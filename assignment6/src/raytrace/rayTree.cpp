#include "raytrace/rayTree.h"
#include <GL/gl.h>

namespace raytrace
{

// ====================================================================
// Initialize the static variables
int RayTree::activated = 0;  
Segment RayTree::main_segment;
SegmentVector RayTree::shadow_segments;
SegmentVector RayTree::reflected_segments;
SegmentVector RayTree::transmitted_segments;
SegmentVector RayTree::normal_segments;
CellFaceVector RayTree::hit_cells;
CellFaceVector RayTree::entered_faces;
constexpr int RayTree::color_gradient_num;

// I use HSL color here
Vec3f RayTree::color_gradient[color_gradient_num] = {
  Vec3f{1, 1, 1},
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 2, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 3, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 4, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 5, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 6, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 7, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 8, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 9, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 10, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 11, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 12, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 13, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 14, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 15, 1, 0.5f}),
  RayTree::HSL2RGB(Vec3f{360 - 360.f / color_gradient_num * 0, 1, 0.5f})
};

// ====================================================================

void RayTree::Print() {
  main_segment.Print("main       ");
  int i;
  for (int i = 0; i < shadow_segments.getNumSegments(); i++) {
    shadow_segments.getSegment(i).Print("shadow     "); }
  for (int i = 0; i < reflected_segments.getNumSegments(); i++) {
    reflected_segments.getSegment(i).Print("reflected  "); }
  for (int i = 0; i < transmitted_segments.getNumSegments(); i++) {
    transmitted_segments.getSegment(i).Print("transmitted"); }
  for (int i = 0; i < normal_segments.getNumSegments(); i++) {
    normal_segments.getSegment(i).Print("normal     "); }
}

void RayTree::paintHelper(const Vec4f &m,const Vec4f &s,const Vec4f &r,const Vec4f &t, const Vec4f &n) {
  glBegin(GL_LINES);
  glColor4f(m.r(),m.g(),m.b(),m.a());
  main_segment.paint();
  int i;
  glColor4f(s.r(),s.g(),s.b(),s.a());
  for (int i = 0; i < shadow_segments.getNumSegments(); i++) {
    shadow_segments.getSegment(i).paint(); }
  glColor4f(r.r(),r.g(),r.b(),r.a());
  for (int i = 0; i < reflected_segments.getNumSegments(); i++) {
    reflected_segments.getSegment(i).paint(); }
  glColor4f(t.r(),t.g(),t.b(),t.a());
  for (int i = 0; i < transmitted_segments.getNumSegments(); i++) {
    transmitted_segments.getSegment(i).paint(); }  
  glColor4f(n.r(),n.g(),n.b(),n.a());
  for (int i = 0; i < normal_segments.getNumSegments(); i++) {
    normal_segments.getSegment(i).paint(); }
  glEnd();
}

// ====================================================================

void RayTree::paint() {

  glLineWidth(2);
  glDisable(GL_LIGHTING);

  // this allows you to see rays passing through objects
  // turn off the depth test and blend with the current pixel color
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_DST_COLOR);
  paintHelper(Vec4f(0.7,0.7,0.7,0.3),
	      Vec4f(0.1,0.9,0.1,0.3),
	      Vec4f(0.9,0.1,0.1,0.3),
	      Vec4f(0.1,0.1,0.9,0.3),
        Vec4f(0.957, 0.957, 0.262, 0.3));
  glDisable(GL_BLEND);
  glEnable(GL_DEPTH_TEST);

  // with the depth test enabled, draw the lines solid
  paintHelper(Vec4f(0.7,0.7,0.7,1.0),
	      Vec4f(0.1,0.9,0.1,1.0),
	      Vec4f(0.9,0.1,0.1,1.0),
	      Vec4f(0.1,0.1,0.9,1.0),
        Vec4f(0.957, 0.957, 0.262, 1.0));
	      
  glEnable(GL_LIGHTING);
}

// ====================================================================

void RayTree::paintHitCells() {
  // paint the affected cells!
  for (int i = 0; i < hit_cells.getNumCellFaces(); i++) {
    hit_cells.getCellFace(i).paint(); }
}

// ====================================================================

void RayTree::paintEnteredFaces() {
  // paint the entered faces
  for (int i = 0; i < entered_faces.getNumCellFaces(); i++) {
    entered_faces.getCellFace(i).paint(); }
}

Vec3f RayTree::HSL2RGB(Vec3f hsl)
{
  float h = hsl[0];
  float s = hsl[1];
  float l = hsl[2];
  float c = (1 - std::abs(2 * l - 1)) * s;
  float x = c * (1 - std::abs(std::fmod(h / 60, 2) - 1));
  float m = l - c / 2;
  float r, g, b;
  if (h < 60)
  {
    r = c;
    g = x;
    b = 0;
  }
  else if (h < 120)
  {
    r = x;
    g = c;
    b = 0;
  }
  else if (h < 180)
  {
    r = 0;
    g = c;
    b = x;
  }
  else if (h < 240)
  {
    r = 0;
    g = x;
    b = c;
  }
  else if (h < 300)
  {
    r = x;
    g = 0;
    b = c;
  }
  else
  {
    r = c;
    g = 0;
    b = x;
  }
  return Vec3f(r + m, g + m, b + m);
}

} // namespace raytrace
// ====================================================================
