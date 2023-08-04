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
    int size = std::min(width, height);
    result.SetAllPixels(scene_parser.getBackgroundColor());
    depth_img.SetAllPixels(Vec3f(0, 0, 0));
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            auto ray = scene_parser.getCamera()->generateRay(Vec2f((float)i / size, (float)j / size));
            bool has_intersect = group->intersect(ray, tmp, scene_parser.getCamera()->getTMin());
            
            if (has_intersect)
            {
                // clamp depth
                float depth = tmp.getT();
                depth = std::min(depth, depth_max);
                depth = std::max(depth, depth_min);
                depth = (depth_max - depth) / (depth_max - depth_min);

                result.SetPixel(i, j, tmp.getMaterial()->getDiffuseColor());
                depth_img.SetPixel(i, j, Vec3f(depth, depth, depth));
            }
        }
    }
    result.SaveTGA(output_file);
    depth_img.SaveTGA(depth_file);
}

