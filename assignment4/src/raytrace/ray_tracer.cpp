#include "raytrace/ray_tracer.h"
#include "utility/vectors.h"
#include "object3ds/scene_parser.h"
#include "object3ds/group.h"
#include "lights/light.h"

namespace raytrace
{
constexpr float epsilon = 0.001f;
Vec3f RayTracer::traceRay(const Ray &ray, float tmin, int bounces, float weight, 
                float indexOfRefraction, Hit &hit) const
{
    // Reinitialize hit to be a miss.
    hit.set(tmin, nullptr, Vec3f(0, 0, 0), ray);

    Vec3f color(0, 0, 0);
    if (bounces > m_max_bounces || weight < m_cutoff_weight)
    {
        return color;
    }

    bool has_intersect = m_scene_parser->getGroup()->intersect(ray, hit, tmin);
    if (has_intersect)
    {
        color = hit.getMaterial()->getDiffuseColor() * m_scene_parser->getAmbientLight();
        for (int iLight = 0; iLight < m_scene_parser->getNumLights(); ++iLight)
        {
            Vec3f dirToLight;
            Vec3f lightColor;
            float distanceToLight;
            m_scene_parser->getLight(iLight)->getIllumination(ray.pointAtParameter(hit.getT()), dirToLight, lightColor, distanceToLight);

            if (m_shadows)
            {
                Ray shadowRay(ray.pointAtParameter(hit.getT()), dirToLight);
                Hit shadowHit;
                bool has_shadow = m_scene_parser->getGroup()->intersect(shadowRay, shadowHit, tmin + epsilon);
                if (!has_shadow) color += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
            }
            else
                color += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
        }
    }

    return color;
}
} // namespace raytrace
