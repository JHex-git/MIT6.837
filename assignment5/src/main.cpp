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
#include <GL/glut.h>

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

SceneParser* scene_parser = nullptr;
RayTracer* ray_tracer = nullptr;

int main(int argc, char *argv[])
{
    bool gui = false;
    bool shadows = false;
    bool visualize_grid = false;
    int bounces = 0;
    float weight = 0.0f;
    int nx = 0, ny = 0, nz = 0;

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
            gui = true;
        } else if (!strcmp(argv[i],"-tessellation")) {
            i++; assert (i < argc); 
            Sphere::theta_steps = atoi(argv[i]);
            i++; assert (i < argc); 
            Sphere::phi_steps = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-gouraud")) {
            Sphere::gouraud = true;
        } else if (!strcmp(argv[i],"-shadows")) {
            shadows = true;
        } else if (!strcmp(argv[i],"-bounces")) {
            i++; assert (i < argc); 
            bounces = atoi(argv[i]);
        } else if (!strcmp(argv[i],"-weight")) {
            i++; assert (i < argc); 
            weight = atof(argv[i]);
        } else if (!strcmp(argv[i],"-visualize_grid")) {
            visualize_grid = true;
        } else if (!strcmp(argv[i], "-grid")) {
            i++; assert (i < argc);
            nx = atoi(argv[i]);
            i++; assert (i < argc);
            ny = atoi(argv[i]);
            i++; assert (i < argc);
            nz = atoi(argv[i]);
        }
        else {
            std::cerr << "whoops error with command line argument " << i << ": '" << argv[i] << "'" << std::endl;
            assert(0);
        }
    }
    
    GLCanvas canvas;
    scene_parser = new SceneParser(input_file);
    ray_tracer = new RayTracer(scene_parser, bounces, weight, shadows, shade_back);
    Grid grid(scene_parser->getGroup()->getBoundingBox(), nx, ny, nz);
    if (gui) canvas.initialize(scene_parser, Render, TraceRay, &grid, visualize_grid);
    else
    {
        Render();
    }
}

void Render()
{
    Group *group = scene_parser->getGroup();
    Hit tmp;
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
            auto ray = scene_parser->getCamera()->generateRay(Vec2f((float)i / size, (float)j / size));
            Vec3f shade_color = ray_tracer->traceRay(ray, scene_parser->getCamera()->getTMin(), 0, 1, 1, tmp); // camera always placed in air, not in object, so index of refraction is 1
            if (tmp.getT() > scene_parser->getCamera()->getTMin()) // has intersect
            {
                Vec3f normal = tmp.getNormal();
                result.SetPixel(i, j, shade_color);

                if (depth_file != NULL) 
                {
                    // clamp depth
                    float depth = tmp.getT();
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
    result.SaveTGA(output_file);
    if (depth_file != NULL) depth_img.SaveTGA(depth_file);
    if (normal_file != NULL) normal_img.SaveTGA(normal_file);
}

void TraceRay(float x, float y)
{
    Hit hit;
    ray_tracer->traceRay(scene_parser->getCamera()->generateRay(Vec2f(x, y)), scene_parser->getCamera()->getTMin(), 0, 1, 1, hit); // camera always placed in air, not in object, so index of refraction is 1
}