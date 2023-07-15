#include "raytrace/ray_tracer.h"
#include "utility/vectors.h"
#include "object3ds/scene_parser.h"
#include "object3ds/group.h"
#include "object3ds/grid.h"
#include "lights/light.h"
#include "raytrace/rayTree.h"

namespace raytrace
{
constexpr float epsilon = 0.001f;

RayTracer::RayTracer(std::shared_ptr<SceneParser> s, std::shared_ptr<Grid> grid, int max_bounces, float cutoff_weight, 
                        bool shadows, bool shade_back, bool visualize_grid)
                        : m_scene_parser(s), m_grid(grid), m_max_bounces(max_bounces), m_cutoff_weight(cutoff_weight), 
                        m_shadows(shadows), m_shade_back(shade_back), m_visualize_grid(visualize_grid)
{
    m_scene_parser->getGroup()->insertIntoGrid(m_grid.get(), nullptr);
}

Vec3f RayTracer::traceRay(const Ray &ray, float tmin, int bounces, float weight, 
                float indexOfRefraction, Hit &hit) const
{
    if (m_visualize_grid) return traceRayGrid(ray, tmin, bounces, weight, indexOfRefraction, hit);
    else return traceRayScene(ray, tmin, bounces, weight, indexOfRefraction, hit);
}

Vec3f RayTracer::traceRayGrid(const Ray &ray, float tmin, int bounces, float weight, 
                    float indexOfRefraction, Hit &hit) const
{
    // Reinitialize hit to be a miss.
    hit.set(tmin, nullptr, Vec3f(0, 0, 0), ray);

    Vec3f color(0, 0, 0);

    RayTree::SetMainSegment(ray, tmin, 100);
    if (m_grid->intersect(ray, hit, tmin))
    {
        // RayTree::SetMainSegment(ray, tmin, hit.getT());
        // Vec3f normal = hit.getNormal();
        // float normal_dot_ray = normal.Dot3(ray.getDirection());
        // if (normal_dot_ray > 0) // at back side
        // {
        //     if (m_shade_back) normal.Negate();
        //     else
        //     {
        //         return Vec3f(0, 0, 0);
        //     }
        // }

        // Material* material = hit.getMaterial();
        // color = material->getDiffuseColor() * m_scene_parser->getAmbientLight();
        // for (int iLight = 0; iLight < m_scene_parser->getNumLights(); ++iLight)
        // {
        //     Vec3f dirToLight;
        //     Vec3f lightColor;
        //     float distanceToLight;
        //     m_scene_parser->getLight(iLight)->getIllumination(ray.pointAtParameter(hit.getT()), dirToLight, lightColor, distanceToLight);

        //     if (m_shadows)
        //     {
        //         Ray shadowRay(ray.pointAtParameter(hit.getT()), dirToLight);
        //         Hit shadowHit;
        //         bool shadow_intersect = m_grid->intersect(shadowRay, shadowHit, epsilon);
        //         // shadow ray may hit light or other objects first, if it hits other objects first, then it is in shadow, otherwise it is not in shadow
        //         float distance2light_obj = distanceToLight;
        //         if (shadow_intersect) distance2light_obj = std::min(shadowHit.getT(), distanceToLight);
        //         RayTree::AddShadowSegment(shadowRay, 0, distance2light_obj);
        //         if (distance2light_obj == distanceToLight) color += material->Shade(ray, hit, dirToLight, lightColor); // hit light first, so no shadow
        //     }
        //     else
        //         color += material->Shade(ray, hit, dirToLight, lightColor);
        // }
            
    }

    return color;
}

Vec3f RayTracer::traceRayScene(const Ray &ray, float tmin, int bounces, float weight, 
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
        if (bounces == 0) RayTree::SetMainSegment(ray, tmin, hit.getT());
        Vec3f normal = hit.getNormal();
        float normal_dot_ray = normal.Dot3(ray.getDirection());
        if (normal_dot_ray > 0) // at back side
        {
            if (m_shade_back) normal.Negate();
            else
            {
                return color;
            }
        }

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
                bool shadow_intersect = m_scene_parser->getGroup()->intersect(shadowRay, shadowHit, epsilon);
                // shadow ray may hit light or other objects first, if it hits other objects first, then it is in shadow, otherwise it is not in shadow
                float distance2light_obj = distanceToLight;
                if (shadow_intersect) distance2light_obj = std::min(shadowHit.getT(), distanceToLight);
                RayTree::AddShadowSegment(shadowRay, 0, distance2light_obj);
                if (distance2light_obj == distanceToLight) color += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor); // hit light first, so no shadow
            }
            else
                color += hit.getMaterial()->Shade(ray, hit, dirToLight, lightColor);
        }

        Ray normal_ray(ray.pointAtParameter(hit.getT()), normal);
        RayTree::AddNormalSegment(normal_ray);
        
        Vec3f reflective_color = hit.getMaterial()->getReflectiveColor();
        if (reflective_color != Vec3f(0, 0, 0))
        {
            Ray reflect_ray(ray.pointAtParameter(hit.getT()), mirrorDirection(normal, ray.getDirection()));
            Hit reflect_hit;
            // color += reflective_color * traceRayScene(reflect_ray, tmin, bounces + 1, weight * reflective_color.Length(), indexOfRefraction, reflect_hit);
            color += reflective_color * traceRayScene(reflect_ray, epsilon, bounces + 1, weight * reflective_color.Length() / std::sqrt(3), indexOfRefraction, reflect_hit);
            if (reflect_hit.getT() != 0)
            {
                RayTree::AddReflectedSegment(reflect_ray, 0, reflect_hit.getT());
            }
        }

        Vec3f transparent_color = hit.getMaterial()->getTransparentColor();
        if (transparent_color != Vec3f(0, 0, 0))
        {
            Vec3f transmitted_direction;
            bool has_transmitted;
            if (normal_dot_ray < 0) // from outside to inside
                has_transmitted = transmittedDirection(normal, ray.getDirection(), 1, hit.getMaterial()->getIndexOfRefraction(), transmitted_direction); // no nested transparent objects
            else // from inside to outside
                has_transmitted = transmittedDirection(normal, ray.getDirection(), hit.getMaterial()->getIndexOfRefraction(), 1, transmitted_direction); // no nested transparent objects
            if (has_transmitted)
            {
                Ray transmitted_ray(ray.pointAtParameter(hit.getT()), transmitted_direction);
                Hit transmitted_hit;
                // transmitted_color = traceRayScene(transmitted_ray, tmin, bounces + 1, weight * transparent_color.Length(), hit.getMaterial()->getIndexOfRefraction(), transmitted_hit);
                color += transparent_color * traceRayScene(transmitted_ray, epsilon, bounces + 1, weight * transparent_color.Length() / std::sqrt(3), hit.getMaterial()->getIndexOfRefraction(), transmitted_hit);
                if (transmitted_hit.getT() != 0)
                {
                    RayTree::AddTransmittedSegment(transmitted_ray, 0, transmitted_hit.getT());
                }
            }
        }
    }
    else
    {
        color = m_scene_parser->getBackgroundColor();
    }

    return color;
    // return color * weight;
}

Vec3f RayTracer::mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const
{
    return incoming - 2 * normal.Dot3(incoming) * normal;
}

bool RayTracer::transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted) const
{
    float index_r = index_i / index_t;
    Vec3f i = incoming;
    Vec3f n = normal;
    i.Negate();
    // if (i.Dot3(n) < 0) n.Negate();
    float discriminant = 1 - index_r * index_r * (1 - n.Dot3(i) * n.Dot3(i));
    if (discriminant < 0) return false; // Total internal reflection

    transmitted = (index_r * n.Dot3(i) - std::sqrt(1 - index_r * index_r * (1 - n.Dot3(i) * n.Dot3(i)))) * n - index_r * i;
    transmitted.Normalize();
    return true;
}
} // namespace raytrace
