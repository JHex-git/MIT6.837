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

void Render();
void TraceRay(float x, float y);

int width = 100;
int height = 100;
char *input_file = NULL;
char *output_file = NULL;
char *depth_file = NULL;
char *normal_file = NULL;
float depth_min = 0;
float depth_max = 1;
bool shade_back = false;
bool stats = false;

std::shared_ptr<SceneParser> scene_parser = nullptr;
std::shared_ptr<RayTracer> ray_tracer = nullptr;

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
        } else {
            std::cerr << "whoops error with command line argument " << i << ": '" << argv[i] << "'" << std::endl;
            assert(0);
        }
    }
    
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
    int size = std::min(width, height);
    result.SetAllPixels(scene_parser->getBackgroundColor());
    if (depth_file != NULL) depth_img.SetAllPixels(Vec3f(0, 0, 0));
    if (normal_file != NULL) normal_img.SetAllPixels(Vec3f(0, 0, 0));
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            auto ray = scene_parser->getCamera()->generateRay(Vec2f((float)i / (size - 1), (float)j / (size - 1)));
            Vec3f shade_color = ray_tracer->traceRay(ray, scene_parser->getCamera()->getTMin(), 0, 1, 1, hit); // camera always placed in air, not in object, so index of refraction is 1
            if (hit.getT() > scene_parser->getCamera()->getTMin()) // has intersect
            {
                Vec3f normal = hit.getNormal();
                result.SetPixel(i, j, shade_color);

                if (depth_file != NULL) 
                {
                    // clamp depth
                    float depth = hit.getT();
                    depth = std::min(depth, depth_max);
                    depth = std::max(depth, depth_min);
                    depth = (depth_max - depth) / (depth_max - depth_min);
                    depth_img.SetPixel(i, j, Vec3f(depth, depth, depth));
                }
                if (normal_file != NULL)
                {
                    Vec3f abs_normal = Vec3f(std::abs(normal.x()), std::abs(normal.y()), std::abs(normal.z()));
                    normal_img.SetPixel(i, j, abs_normal);
                }
            }
        }
    }
    if (stats) RayTracingStats::PrintStatistics();
    result.SaveTGA(output_file);
    if (depth_file != NULL) depth_img.SaveTGA(depth_file);
    if (normal_file != NULL) normal_img.SaveTGA(normal_file);
}

void TraceRay(float x, float y)
{
    Hit hit;
    ray_tracer->traceRay(scene_parser->getCamera()->generateRay(Vec2f(x, y)), scene_parser->getCamera()->getTMin(), 0, 1, 1, hit); // camera always placed in air, not in object, so index of refraction is 1
}