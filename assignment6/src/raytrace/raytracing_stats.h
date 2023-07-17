#ifndef _RAYTRACING_STATS_H_
#define _RAYTRACING_STATS_H_

#include <time.h>
#include <limits.h>
#include <memory>

#include "utility/vectors.h"
#include "object3ds/boundingbox.h"

// ====================================================================
// ====================================================================

// This class only contains static variables and static member
// functions so there is no need to call the constructor, destructor
// etc.  It's just a wrapper for the global statistics data.
namespace object3ds
{
  class BoundingBox;
} // namespace object3ds

namespace raytrace
{
using object3ds::BoundingBox;

class RayTracingStats {

public:

  // To be called before computation begins
  static void Initialize(int _width, int _height, std::shared_ptr<BoundingBox> _bbox, 
			 int nx, int ny, int nz);

  // Call for each non-shadow ray
  static void IncrementNumNonShadowRays() { num_nonshadow_rays++; }

  // Call for each shadow ray
  static void IncrementNumShadowRays() { num_shadow_rays++; }

  // Add this to each Object3D primitive's intersect routine 
  // (but not group and transform). 
  // This is a count of the number of times ray-primitive intersection
  // is computed, not the number of times it returns true
  static void IncrementNumIntersections() {    
    // WARNING:  this might overflow
    num_intersections++; }

  // Call each time a new cell is entered by a ray
  static void IncrementNumGridCellsTraversed() { num_grid_cells_traversed++; }

  // Call when you're all done
  static void PrintStatistics();

private:

  // TIMING VARIABLES
  // use long longs to try to avoid overflow problems, but with really
  // big, complex scenes we will still have problems.
  static int width;
  static int height;
  static std::shared_ptr<BoundingBox> bbox;
  static int num_x;
  static int num_y;
  static int num_z;
  static unsigned long long start_time;
  static unsigned long long num_nonshadow_rays;
  static unsigned long long num_shadow_rays;
  static unsigned long long num_intersections;
  static unsigned long long num_grid_cells_traversed;  
};

// ====================================================================
// ====================================================================
} // namespace raytrace


#endif
