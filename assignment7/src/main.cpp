#include <cassert>
#include <cstring>
#include <iostream>
#include "object3ds/scene_parser.h"
#include "cameras/orthographic_camera.h"
#include "utility/vectors.h"
#include "utility/image.h"
#include "object3ds/group.h"
#include "object3ds/sphere.h"
#include "object3ds/grid.h"
#include "raytrace/hit.h"
#include "materials/material.h"
#include "lights/light.h"
#include "opengl/glCanvas.h"
#include "raytrace/ray_tracer.h"
#include "raytrace/raytracing_stats.h"
#include "samples/sampler.h"
#include "samples/film.h"
#include "samples/filter.h"
#include <GL/glut.h>


#include "object3ds/boundingbox.h"
// ========================================================
// ========================================================
// ========================================================
// ========================================================
using object3ds::SceneParser;
using object3ds::Group;
using object3ds::Sphere;
using object3ds::Grid;
using cameras::Camera;
using cameras::OrthographicCamera;
using utility::Vec2f;
using utility::Vec3f;
using utility::Image;
using raytrace::Ray;
using raytrace::Hit;
using raytrace::RayTracer;
using raytrace::RayTracingStats;
using materials::Material;
using lights::Light;
using opengl::GLCanvas;
using samples::Sampler;
using samples::RandomSampler;
using samples::UniformSampler;
using samples::JitteredSampler;
using samples::Film;
using samples::Filter;
using samples::BoxFilter;
using samples::TentFilter;
using samples::GaussianFilter;


void Render();
void TraceRay(float x, float y);
// void RenderWithoutSample();
// void RenderWithSample();

int width = 100;
int height = 100;
int sample_zoom = 1;
int filter_zoom = 1;
char *input_file = NULL;
char *output_file = NULL;
char *depth_file = NULL;
char *normal_file = NULL;
char *samples_file = NULL;
char *filter_file = NULL;
float depth_min = 0;
float depth_max = 1;
bool shade_back = false;
bool stats = false;

std::shared_ptr<SceneParser> scene_parser = nullptr;
std::shared_ptr<RayTracer> ray_tracer = nullptr;
std::shared_ptr<Sampler> sampler = nullptr;
std::shared_ptr<Filter> filter = nullptr;

int main(int argc, char *argv[])
{
    bool b_gui = false;
    bool b_shadows = false;
    bool b_visualize_grid = false;
    bool b_grid = false;
    int bounces = 0;
    float weight = 0.0f;
    int nx = -1, ny = -1, nz = -1;

    glutInit(&argc, argv);
    // sample command line:
    // raytracer -input scene1_1.txt -size 200 200 -output output1_1.tga -depth 9 10 depth1_1.tga
    for (int i = 1; i < argc; i++) {
        if (!strcmp(argv[i],"-input")) {
            i++; assert (i < argc); 
            input_file = argv[i];
        } else if (!strcmp(argv[i],"-size")) {
            i++; assert (i < argc); 
            width = atoi(argv[i]);
            i++; assert (i < argc); 
            height = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-output")) {
            i++; assert (i < argc); 
            output_file = argv[i];
        } else if (!strcmp(argv[i],"-depth")) {
            i++; assert (i < argc); 
            depth_min = atof(argv[i]);
            i++; assert (i < argc); 
            depth_max = atof(argv[i]);
            i++; assert (i < argc); 
            depth_file = argv[i];
        } else if (!strcmp(argv[i],"-normals")) {
            i++; assert (i < argc); 
            normal_file = argv[i];
        } else if (!strcmp(argv[i],"-shade_back")) {
            shade_back = true;
        } else if (!strcmp(argv[i],"-gui")) {
            b_gui = true;
        } else if (!strcmp(argv[i],"-tessellation")) {
            i++; assert (i < argc); 
            Sphere::theta_steps = atoi(argv[i]);
            i++; assert (i < argc); 
            Sphere::phi_steps = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-gouraud")) {
            Sphere::gouraud = true;
        } else if (!strcmp(argv[i],"-shadows")) {
            b_shadows = true;
        } else if (!strcmp(argv[i],"-bounces")) {
            i++; assert (i < argc); 
            bounces = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-weight")) {
            i++; assert (i < argc); 
            weight = atof(argv[i]);
        } else if (!strcmp(argv[i],"-visualize_grid")) {
            b_visualize_grid = true;
        } else if (!strcmp(argv[i], "-grid")) {
            b_grid = true;
            i++; assert (i < argc);
            nx = atoi(argv[i]);
            i++; assert (i < argc);
            ny = atoi(argv[i]);
            i++; assert (i < argc);
            nz = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-stats")) {
            stats = true;
        } else if (!strcmp(argv[i],"-random_samples")) {
            assert (sampler == nullptr);
            i++; assert (i < argc);
            sampler = std::make_shared<RandomSampler>(atoi(argv[i]));
        } else if (!strcmp(argv[i],"-uniform_samples")) {
            assert (sampler == nullptr);
            i++; assert (i < argc);
            sampler = std::make_shared<UniformSampler>(atoi(argv[i]));
        } else if (!strcmp(argv[i],"-jittered_samples")) {
            assert (sampler == nullptr);
            i++; assert (i < argc);
            sampler = std::make_shared<JitteredSampler>(atoi(argv[i]));
        } else if (!strcmp(argv[i],"-box_filter")) {
            assert (filter == nullptr);
            i++; assert (i < argc);
            filter = std::make_shared<BoxFilter>(atof(argv[i]));
        } else if (!strcmp(argv[i],"-tent_filter")) {
            assert (filter == nullptr);
            i++; assert (i < argc);
            filter = std::make_shared<TentFilter>(atof(argv[i]));
        } else if (!strcmp(argv[i],"-gaussian_filter")) {
            assert (filter == nullptr);
            i++; assert (i < argc);
            filter = std::make_shared<GaussianFilter>(atof(argv[i]));
        } else if (!strcmp(argv[i],"-render_samples")) {
            i++; assert (i < argc);
            samples_file = argv[i];
            i++; assert (i < argc);
            sample_zoom = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-render_filter")) {
            i++; assert (i < argc);
            filter_file = argv[i];
            i++; assert (i < argc);
            filter_zoom = atoi(argv[i]);
        } else {
            std::cerr << "whoops error with command line argument " << i << ": '" << argv[i] << "'" << std::endl;
            assert(0);
        }
    }

    if (sampler == nullptr) sampler = std::make_shared<UniformSampler>(1);
    if (filter == nullptr) filter = std::make_shared<BoxFilter>(0.5);
    GLCanvas canvas;
    scene_parser = std::make_shared<SceneParser>(input_file);
    ray_tracer = std::make_shared<RayTracer>(scene_parser, bounces, weight, b_shadows, shade_back, b_grid, nx, ny, nz, b_visualize_grid);
    RayTracingStats::Initialize(width, height, scene_parser->getGroup()->getBoundingBox(), nx, ny, nz);
    if (b_gui) canvas.initialize(scene_parser, Render, TraceRay, ray_tracer->getGrid(), b_visualize_grid);
    else
    {
        Render();
    }
}

void Render()
{
    Hit hit;
    Image result(width, height);
    Image depth_img(width, height);
    Image normal_img(width, height);
    Film result_film(width, height, sampler->getNumSamplesPerPixel());
    Film depth_film(width, height, sampler->getNumSamplesPerPixel());
    Film normal_film(width, height, sampler->getNumSamplesPerPixel());

    int size = std::max(width, height);
    int i_begin = width < height ? (size - width) / 2 : 0;
    int j_begin = width > height ? (size - height) / 2 : 0;
    int samples_per_pixel = sampler->getNumSamplesPerPixel();
    float pixel_size = (float)1 / size;

    for (int i = i_begin; i < size - i_begin; i++)
    {
        for (int j = j_begin; j < size - j_begin; j++)
        {
            for (int k = 0; k < samples_per_pixel; k++)
            {
                auto offset = sampler->getSamplePosition(k);
                auto ray_offset = offset;
                ray_offset -= Vec2f(0.5, 0.5); // [-0.5, 0.5]
                ray_offset *= pixel_size;
                
                auto ray = scene_parser->getCamera()->generateRay(Vec2f((i + 0.5) / size + ray_offset.x(), (j + 0.5) / size + ray_offset.y()));
                Vec3f shade_color = ray_tracer->traceRay(ray, scene_parser->getCamera()->getTMin(), 0, 1, 1, hit); // camera always placed in air, not in object, so index of refraction is 1
                if (hit.getT() > scene_parser->getCamera()->getTMin()) // has intersect
                {
                    if (output_file != NULL || samples_file != NULL) result_film.setSample(i - i_begin, j - j_begin, k, offset, shade_color);

                    if (depth_file != NULL) 
                    {
                        // clamp depth
                        float depth = hit.getT();
                        depth = std::min(depth, depth_max);
                        depth = std::max(depth, depth_min);
                        depth = (depth_max - depth) / (depth_max - depth_min);
                        depth_film.setSample(i - i_begin, j - j_begin, k, offset, Vec3f(depth, depth, depth));
                    }
                    if (normal_file != NULL)
                    {
                        Vec3f normal = hit.getNormal();
                        Vec3f abs_normal = Vec3f(std::abs(normal.x()), std::abs(normal.y()), std::abs(normal.z()));
                        normal_film.setSample(i - i_begin, j - j_begin, k, offset, abs_normal);
                    }
                }
                else
                {
                    if (output_file != NULL || samples_file != NULL) result_film.setSample(i - i_begin, j - j_begin, k, offset, scene_parser->getBackgroundColor());
                    if (depth_file != NULL) depth_film.setSample(i - i_begin, j - j_begin, k, offset, Vec3f(0, 0, 0));
                    if (normal_file != NULL) normal_film.setSample(i - i_begin, j - j_begin, k, offset, Vec3f(0, 0, 0));
                }
            }
        }
    }
    if (samples_file != NULL) result_film.renderSamples(samples_file, sample_zoom);
    if (filter_file != NULL) result_film.renderFilter(filter_file, filter_zoom, filter.get());
    if (output_file != NULL || depth_file != NULL || normal_file != NULL)
    {
        for (int i = 0; i < width; i++)
        {
            for (int j = 0; j < height; ++j)
            {
                if (output_file != NULL)
                {
                    Vec3f color = filter->getColor(i, j, &result_film);
                    result.SetPixel(i, j, color);
                }
                if (depth_file != NULL)
                {
                    Vec3f color = filter->getColor(i, j, &depth_film);
                    depth_img.SetPixel(i, j, color);
                }
                if (normal_file != NULL)
                {
                    Vec3f color = filter->getColor(i, j, &normal_film);
                    normal_img.SetPixel(i, j, color);
                }
            }
        }
        if (output_file != NULL) result.SaveTGA(output_file);
        if (depth_file != NULL) depth_img.SaveTGA(depth_file);
        if (normal_file != NULL) normal_img.SaveTGA(normal_file);
    }
    if (stats) RayTracingStats::PrintStatistics();
}

void TraceRay(float x, float y)
{
    Hit hit;
    ray_tracer->traceRay(scene_parser->getCamera()->generateRay(Vec2f(x, y)), scene_parser->getCamera()->getTMin(), 0, 1, 1, hit); // camera always placed in air, not in object, so index of refraction is 1
}