#ifndef _RAY_TRACE_H_
#define _RAY_TRACE_H_
#include <memory>

namespace object3ds
{
    class SceneParser;
    class Grid;
} // namespace object3ds

namespace utility
{
    class Vec3f;
} // namespace utility


namespace raytrace
{
class Ray;
class Hit;

using object3ds::SceneParser;
using object3ds::Grid;
using utility::Vec3f;
  
class RayTracer
{
public:
    RayTracer(std::shared_ptr<SceneParser> s, std::shared_ptr<Grid> grid, int max_bounces, float cutoff_weight, 
                bool shadows, bool shade_back, bool visualize_grid);

    Vec3f traceRay(const Ray &ray, float tmin, int bounces, float weight, 
                    float indexOfRefraction, Hit &hit) const;

private:
    Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const;
    bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, 
                                float index_i, float index_t, Vec3f &transmitted) const;
    Vec3f traceRayGrid(const Ray &ray, float tmin, int bounces, float weight, 
                    float indexOfRefraction, Hit &hit) const;
    Vec3f traceRayScene(const Ray &ray, float tmin, int bounces, float weight, 
                    float indexOfRefraction, Hit &hit) const;

    std::shared_ptr<SceneParser> m_scene_parser;
    std::shared_ptr<Grid> m_grid;
    int m_max_bounces;
    float m_cutoff_weight;
    bool m_shadows;
    bool m_shade_back;
    bool m_visualize_grid;
};
} // namespace raytrace

#endif