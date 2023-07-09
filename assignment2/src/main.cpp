#include <cassert>
#include <cstring>
#include <iostream>
#include "object3ds/scene_parser.h"
#include "cameras/orthographic_camera.h"
#include "utility/vectors.h"
#include "utility/image.h"
#include "object3ds/group.h"
#include "raytrace/hit.h"
#include "materials/material.h"

// ========================================================
// ========================================================
// ========================================================
// ========================================================
using object3ds::SceneParser;
using object3ds::Group;
using cameras::Camera;
using cameras::OrthographicCamera;
using utility::Vec2f;
using utility::Vec3f;
using utility::Image;
using raytrace::Ray;
using raytrace::Hit;
using materials::Material;

char *input_file = NULL;
int width = 100;
int height = 100;
char *output_file = NULL;
float depth_min = 0;
float depth_max = 1;
char *depth_file = NULL;
char *normal_file = NULL;

int main(int argc, char *argv[])
{
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
        } else {
            std::cerr << "whoops error with command line argument " << i << ": '" << argv[i] << "'" << std::endl;
            assert(0);
        }
    }
    
    SceneParser scene_parser(input_file);
    Group *group = scene_parser.getGroup();
    Hit tmp;
    Image result(width, height);
    Image depth_img(width, height);
    Image normal_img(width, height);
    int size = std::min(width, height);
    result.SetAllPixels(scene_parser.getBackgroundColor());
    if (depth_file != NULL) depth_img.SetAllPixels(Vec3f(0, 0, 0));
    if (normal_file != NULL) normal_img.SetAllPixels(Vec3f(0, 0, 0));
    for (float i = 0; i <= 1; i += 1.0 / size)
    {
        for (float j = 0; j <= 1; j+= 1.0 / size)
        {
            auto ray = scene_parser.getCamera()->generateRay(Vec2f(i, j));
            bool has_intersect = group->intersect(ray, tmp, scene_parser.getCamera()->getTMin());
            
            if (has_intersect)
            {
                result.SetPixel(i * width, j * height, tmp.getMaterial()->getDiffuseColor());

                if (depth_file != NULL) 
                {
                    // clamp depth
                    float depth = tmp.getT();
                    depth = std::min(depth, depth_max);
                    depth = std::max(depth, depth_min);
                    depth = (depth_max - depth) / (depth_max - depth_min);
                    depth_img.SetPixel(i * width, j * height, Vec3f(depth, depth, depth));
                }
                if (normal_file != NULL) normal_img.SetPixel(i * width, j * height,tmp.getNormal());
            }
        }
    }
    result.SaveTGA(output_file);
    if (depth_file != NULL) depth_img.SaveTGA(depth_file);
    if (normal_file != NULL) normal_img.SaveTGA(normal_file);
}

