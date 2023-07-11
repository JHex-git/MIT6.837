#ifndef _RAY_TRACE_H_
#define _RAY_TRACE_H_

namespace object3ds
{
    class SceneParser;
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
using utility::Vec3f;
  
class RayTracer
{
public:
    RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows)
        : m_scene_parser(s), m_max_bounces(max_bounces), m_cutoff_weight(cutoff_weight), m_shadows(shadows) {}

    Vec3f traceRay(const Ray &ray, float tmin, int bounces, float weight, 
                    float indexOfRefraction, Hit &hit) const;

private:
    SceneParser *m_scene_parser;
    int m_max_bounces;
    float m_cutoff_weight;
    bool m_shadows;
};
} // namespace raytrace

#endif