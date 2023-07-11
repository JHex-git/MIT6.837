#include "raytrace/ray_tracer.h"
#include "utility/vectors.h"
#include "object3ds/scene_parser.h"
#include "object3ds/group.h"
#include "lights/light.h"
#include "raytrace/rayTree.h"

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
        RayTree::SetMainSegment(ray, tmin, hit.getT());
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
                RayTree::AddShadowSegment(shadowRay, tmin, has_shadow ? shadowHit.getT() : distanceToLight);
                if (!has_shadow) color += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
            }
            else
                color += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
        }

        Vec3f reflective_color = hit.getMaterial()->getReflectiveColor();
        if (reflective_color != Vec3f(0, 0, 0))
        {
            Ray reflect_ray(ray.pointAtParameter(hit.getT() - epsilon), mirrorDirection(hit.getNormal(), ray.getDirection()));
            Hit reflect_hit;
            color += reflective_color * traceRay(reflect_ray, tmin, bounces + 1, weight * reflective_color.Length(), indexOfRefraction, reflect_hit);
            RayTree::AddReflectedSegment(reflect_ray, tmin, reflect_hit.getT());
        }
    }

    return color * weight;
}

Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const
{
    return incoming - 2 * normal.Dot3(incoming) * normal;
}
} // namespace raytrace
