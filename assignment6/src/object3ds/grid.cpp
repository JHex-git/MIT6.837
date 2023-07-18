#include "object3ds/grid.h"
#include "object3ds/boundingbox.h"
#include "utility/vectors.h"
#include "materials/phong_material.h"
#include "raytrace/raytracing_stats.h"
#include "opengl/glCanvas.h"
#include <GL/gl.h>

namespace object3ds
{
using raytrace::RayTree;
using raytrace::RayTracingStats;
using opengl::GLCanvas;

constexpr float epsilon = 0.001f;

Grid::Grid(std::shared_ptr<BoundingBox> bb, int nx, int ny, int nz, bool visualize_grid) : m_nx(nx), m_ny(ny), m_nz(nz), 
            m_voxel_objects(nx, std::vector<std::vector<std::vector<std::shared_ptr<Object3D>>>>(
                ny, std::vector<std::vector<std::shared_ptr<Object3D>>>(nz, std::vector<std::shared_ptr<Object3D>>()))), m_visualize_grid(visualize_grid)
{
    m_bounding_box = bb;
    Vec3f min = m_bounding_box->getMin();
    Vec3f max = m_bounding_box->getMax();
    auto diff = max - min;
    m_dx = diff.x() / m_nx;
    m_dy = diff.y() / m_ny;
    m_dz = diff.z() / m_nz;
    m_material = new materials::PhongMaterial(Vec3f(1, 1, 1), Vec3f(0, 0, 0), 1, Vec3f(1, 1, 1), Vec3f(1, 1, 1), 1);

    for (int i = 0; i < RayTree::color_gradient_num; ++i)
    {
        cell_materials[i] = new materials::PhongMaterial(RayTree::color_gradient[i], Vec3f(0, 0, 0), 1, Vec3f(1, 1, 1), Vec3f(1, 1, 1), 1);
    }
}

Grid::~Grid()
{
    delete m_material;
    for (int i = 0; i < RayTree::color_gradient_num; ++i)
    {
        delete cell_materials[i];
    }
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{
    constexpr float epsilon = 0.0001f;
    initializeRayMarch(m_mi, r, tmin);
    auto voxel_index = m_mi.getVoxelIndex();
    if (m_visualize_grid)
    {
        int face_material_index = 0;
        bool b_first_hit = false;
        while (voxel_index[0] > -1 && voxel_index[1] > -1 && voxel_index[2] > -1 && voxel_index[0] < m_nx && voxel_index[1] < m_ny && voxel_index[2] < m_nz)
        {
            RayTracingStats::IncrementNumGridCellsTraversed();
            Material* face_material = cell_materials[face_material_index];
            Vec3f normal = m_mi.getCrossFaceNormal();
            auto enter_face_points = getFacePoints(voxel_index[0], voxel_index[1], voxel_index[2], normal);
            RayTree::AddEnteredFace(enter_face_points[0], enter_face_points[1], enter_face_points[2], enter_face_points[3], normal, face_material);

            normal = Vec3f(1, 0, 0);
            auto hit_face_points = getFacePoints(voxel_index[0], voxel_index[1], voxel_index[2], normal);
            RayTree::AddHitCellFace(hit_face_points[0], hit_face_points[1], hit_face_points[2], hit_face_points[3], normal, face_material);
            normal = Vec3f(-1, 0, 0);
            hit_face_points = getFacePoints(voxel_index[0], voxel_index[1], voxel_index[2], normal);
            RayTree::AddHitCellFace(hit_face_points[0], hit_face_points[1], hit_face_points[2], hit_face_points[3], normal, face_material);
            normal = Vec3f(0, 1, 0);
            hit_face_points = getFacePoints(voxel_index[0], voxel_index[1], voxel_index[2], normal);
            RayTree::AddHitCellFace(hit_face_points[0], hit_face_points[1], hit_face_points[2], hit_face_points[3], normal, face_material);
            normal = Vec3f(0, -1, 0);
            hit_face_points = getFacePoints(voxel_index[0], voxel_index[1], voxel_index[2], normal);
            RayTree::AddHitCellFace(hit_face_points[0], hit_face_points[1], hit_face_points[2], hit_face_points[3], normal, face_material);
            normal = Vec3f(0, 0, 1);
            hit_face_points = getFacePoints(voxel_index[0], voxel_index[1], voxel_index[2], normal);
            RayTree::AddHitCellFace(hit_face_points[0], hit_face_points[1], hit_face_points[2], hit_face_points[3], normal, face_material);
            normal = Vec3f(0, 0, -1);
            hit_face_points = getFacePoints(voxel_index[0], voxel_index[1], voxel_index[2], normal);
            RayTree::AddHitCellFace(hit_face_points[0], hit_face_points[1], hit_face_points[2], hit_face_points[3], normal, face_material);
            
            normal = m_mi.getCrossFaceNormal();
            if (m_voxel_objects[voxel_index[0]][voxel_index[1]][voxel_index[2]].size() != 0 && !b_first_hit)
            {
                b_first_hit = true;
                h.set(m_mi.getTmin(), cell_materials[std::min((int)m_voxel_objects[voxel_index[0]][voxel_index[1]][voxel_index[2]].size(), RayTree::color_gradient_num) - 1], normal, r);
            }
            m_mi.nextCell();
            voxel_index = m_mi.getVoxelIndex();
            face_material_index = (face_material_index + 1) % RayTree::color_gradient_num;
        }
        return b_first_hit;
    }
    else
    {
        Hit temp_non_voxel_hit;
        Hit non_voxel_hit;
        bool non_voxel_intersect = false;
        float current_non_voxel_tmin = std::numeric_limits<float>::max();
        for (auto obj : m_non_voxel_objects)
        {
            if (obj->intersect(r, temp_non_voxel_hit, tmin))
            {
                float t = temp_non_voxel_hit.getT();
                if (t < current_non_voxel_tmin)
                {
                    non_voxel_hit = temp_non_voxel_hit;
                    current_non_voxel_tmin = t;
                    non_voxel_intersect = true;
                }
            }
        }

        Hit temp_hit;
        while (voxel_index[0] > -1 && voxel_index[1] > -1 && voxel_index[2] > -1 && voxel_index[0] < m_nx && voxel_index[1] < m_ny && voxel_index[2] < m_nz)
        {
            RayTracingStats::IncrementNumGridCellsTraversed();

            if (m_voxel_objects[voxel_index[0]][voxel_index[1]][voxel_index[2]].size() != 0)
            {
                auto objects = m_voxel_objects[voxel_index[0]][voxel_index[1]][voxel_index[2]];
                float min = m_mi.getTmin();

                if (min > current_non_voxel_tmin)
                {
                    h = non_voxel_hit;
                    return true;
                }

                float max = m_mi.getNextTmin();
                bool intersect_with_object = false;
                for (auto obj : objects)
                {
                    if (obj->intersect(r, temp_hit, min))
                    {
                        float t = temp_hit.getT();
                        if (t >= min - epsilon && t <= max + epsilon)
                        {
                            h = temp_hit;
                            max = t; // preserve the closest intersection
                            intersect_with_object = true;
                        }
                    }
                }
                if (intersect_with_object && max < current_non_voxel_tmin) // intersection with objects in voxel is closer than non-voxel objects
                {
                    return true;
                }
                else if (intersect_with_object && max > current_non_voxel_tmin) // intersection with non-voxel objects is closer than objects in voxel
                {
                    h = non_voxel_hit;
                    return true;
                }
            }
            m_mi.nextCell();
            voxel_index = m_mi.getVoxelIndex();
        }

        if (non_voxel_intersect)
        {
            h = non_voxel_hit;
            return true;
        }
    }

    return false;
}

void Grid::paint(void) const
{
    // m_material->glSetMaterial();
    glBegin(GL_QUADS);
    for (int i = 0; i < m_nx; ++i)
    {
        for (int j = 0; j < m_ny; ++j)
        {
            for (int k = 0; k < m_nz; ++k)
            {
                if (m_voxel_objects[i][j][k].size() != 0)
                {
                    cell_materials[std::min((int)m_voxel_objects[i][j][k].size(), RayTree::color_gradient_num) - 1]->glSetMaterial();
                    Vec3f center = getVoxelCenter(i, j, k);
                    Vec3f normal;
                    Vec3f p1, p2, p3, p4;
                    
                    // back
                    p1 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    p2 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    p3 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    p4 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    normal = Vec3f(0, 0, -1);
                    // normal = Vec3f(0, -1, 0);
                    glNormal3f(normal[0], normal[1], normal[2]);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glVertex3f(p3[0], p3[1], p3[2]);
                    glVertex3f(p4[0], p4[1], p4[2]);

                    // front
                    p1 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    p2 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    p3 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    p4 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    normal = Vec3f(0, 0, 1);
                    // normal = Vec3f(0, -1, 0);
                    glNormal3f(normal[0], normal[1], normal[2]);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glVertex3f(p3[0], p3[1], p3[2]);
                    glVertex3f(p4[0], p4[1], p4[2]);

                    // left
                    p1 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    p2 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    p3 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    p4 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    normal = Vec3f(-1, 0, 0);
                    // normal = Vec3f(0, -1, 0);
                    glNormal3f(normal[0], normal[1], normal[2]);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glVertex3f(p3[0], p3[1], p3[2]);
                    glVertex3f(p4[0], p4[1], p4[2]);

                    // right
                    p1 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    p2 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    p3 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    p4 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    normal = Vec3f(1, 0, 0);
                    // normal = Vec3f(0, -1, 0);
                    glNormal3f(normal[0], normal[1], normal[2]);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glVertex3f(p3[0], p3[1], p3[2]);
                    glVertex3f(p4[0], p4[1], p4[2]);

                    // The order of points matters as opengl use winder order to determine front-facing or back-facing
                    // top
                    p1 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    p2 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    p3 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    p4 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    normal = Vec3f(0, 1, 0);
                    glNormal3f(normal[0], normal[1], normal[2]);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glVertex3f(p3[0], p3[1], p3[2]);
                    glVertex3f(p4[0], p4[1], p4[2]);

                    // bottom
                    p1 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    p2 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    p3 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    p4 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    normal = Vec3f(0, -1, 0);
                    glNormal3f(normal[0], normal[1], normal[2]);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glVertex3f(p3[0], p3[1], p3[2]);
                    glVertex3f(p4[0], p4[1], p4[2]);
                }
            }
        }
    }
    glEnd();
}

Vec3f Grid::getVoxelCenter(int x, int y, int z) const
{
    Vec3f min = m_bounding_box->getMin();
    Vec3f max = m_bounding_box->getMax();
    return Vec3f(min.x() + (x + 0.5) * m_dx, min.y() + (y + 0.5) * m_dy, min.z() + (z + 0.5) * m_dz);
}

std::vector<int> Grid::getVoxelIndex(const Vec3f &p) const
{
    Vec3f min = m_bounding_box->getMin();
    Vec3f max = m_bounding_box->getMax();


    // for pointer outside the bounding box, return (-1, -1, -1)
    if (p.x() < min.x() - epsilon || p.x() > max.x() + epsilon || p.y() < min.y() - epsilon || p.y() > max.y() + epsilon || p.z() < min.z() - epsilon || p.z() > max.z() + epsilon)
        return std::vector<int>{-1, -1, -1};

    // for edge corner, clamp it to the nearest valid voxel
    int x = (p.x() - min.x()) / m_dx;
    x = std::min(x, m_nx - 1);
    x = std::max(0, x);
    int y = (p.y() - min.y()) / m_dy;
    y = std::min(y, m_ny - 1);
    y = std::max(0, y);
    int z = (p.z() - min.z()) / m_dz;
    z = std::min(z, m_nz - 1);
    z = std::max(0, z);
    return std::vector<int>{x, y, z};
}

bool Grid::intersectBox(const Ray &r, Hit &h, Vec3f min, Vec3f max, float tmin) const
{
    Vec3f r_dir = r.getDirection();
    Vec3f r_orig = r.getOrigin();
    Vec3f normal1(-1, 0, 0), normal2(1, 0, 0);
    Vec3f normal_near, normal_far;

    constexpr float epsilon = 0.0001f;

    if (r_dir.x() == 0 && (r_orig.x() < min.x() - epsilon || r_orig.x() > max.x() + epsilon))
        return false;
    float t1 = std::numeric_limits<float>::lowest();
    float t2 = std::numeric_limits<float>::lowest();
    float t_near = 0;
    float t_far = std::numeric_limits<float>::max();
    if (!(r_dir.x() == 0 && (std::abs(r_orig.x() - min.x()) < epsilon || std::abs(r_orig.x() - max.x()) < epsilon)))
    {
        t1 = (min.x() - r_orig.x()) / r_dir.x();
        t2 = (max.x() - r_orig.x()) / r_dir.x();
        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
            normal1 = Vec3f(1, 0, 0);
            normal2 = Vec3f(-1, 0, 0);
        }

        t_near = t1, t_far = t2;
        normal_near = normal1;
        normal_far = normal2;
    }

    if (r_dir.y() == 0 && (r_orig.y() < min.y() - epsilon || r_orig.y() > max.y() + epsilon))
        return false;
    if (!(r_dir.y() == 0 && (std::abs(r_orig.y() - min.y()) < epsilon || std::abs(r_orig.y() - max.y()) < epsilon)))
    {
        t1 = (min.y() - r_orig.y()) / r_dir.y();
        t2 = (max.y() - r_orig.y()) / r_dir.y();
        normal1 = Vec3f(0, -1, 0);
        normal2 = Vec3f(0, 1, 0);
        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
            normal1 = Vec3f(0, 1, 0);
            normal2 = Vec3f(0, -1, 0);
        }
        normal_near = t1 > t_near ? normal1 : normal_near;
        normal_far = t2 < t_far ? normal2 : normal_far;
        t_near = std::max(t1, t_near);
        t_far = std::min(t2, t_far);
        if (t_near > t_far || t_far < tmin) // box is missed or box is behind
            return false;
    }

    if (r_dir.z() == 0 && (r_orig.z() < min.z() - epsilon || r_orig.z() > max.z() + epsilon))
        return false;
    if (!(r_dir.z() == 0 && (std::abs(r_orig.z() - min.z()) < epsilon || std::abs(r_orig.z() - max.z()) < epsilon)))
    {
        t1 = (min.z() - r_orig.z()) / r_dir.z();
        t2 = (max.z() - r_orig.z()) / r_dir.z();
        normal1 = Vec3f(0, 0, -1);
        normal2 = Vec3f(0, 0, 1);
        if (t1 > t2)
        {
            float temp = t1;
            t1 = t2;
            t2 = temp;
            normal1 = Vec3f(0, 0, 1);
            normal2 = Vec3f(0, 0, -1);
        }
        normal_near = t1 > t_near ? normal1 : normal_near;
        normal_far = t2 < t_far ? normal2 : normal_far;
        t_near = std::max(t1, t_near);
        t_far = std::min(t2, t_far);
        if (t_near > t_far || t_far < tmin) // box is missed or box is behind
            return false;
    }

    if (t1 == std::numeric_limits<float>::lowest() || t2 == std::numeric_limits<float>::lowest()) return false;

    float t = t_near > tmin ? t_near : t_far;
    Vec3f normal = t_near > tmin ? normal_near : normal_far;
    h.set(t, m_material, normal, r);
    return true;
}

void Grid::initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const
{
    Vec3f min = m_bounding_box->getMin();
    Vec3f max = m_bounding_box->getMax();
    Vec3f start_point = r.getOrigin() + tmin * r.getDirection();

    Vec3f start_point_in_bb;
    Vec3f normal(-2, -2, -2);
    Hit hit;
    if (start_point.x() < min.x() || start_point.x() > max.x() || start_point.y() < min.y() || 
        start_point.y() > max.y() || start_point.z() < min.z() || start_point.z() > max.z()) // outside the bounding box
    {
        if (intersectBox(r, hit, min, max, tmin))
        {
            start_point_in_bb = r.pointAtParameter(hit.getT());
            normal = hit.getNormal();
        }
        else
        {
            mi.setVoxelIndex(-1, -1, -1);
            return;
        }
    }
    else
    {
        start_point_in_bb = start_point;
    }

    auto start_point_index = getVoxelIndex(start_point_in_bb);
    if (start_point_index[0] != -1)
    {
        Vec3f center = getVoxelCenter(start_point_index[0], start_point_index[1], start_point_index[2]);
        min = center - Vec3f(m_dx / 2, m_dy / 2, m_dz / 2);
        max = center + Vec3f(m_dx / 2, m_dy / 2, m_dz / 2);
        if (intersectBox(r, hit, min, max, tmin))
        {
            mi.setVoxelIndex(start_point_index[0], start_point_index[1], start_point_index[2]);
            // When a ray hit the edge of a voxel, it cannot judge the normal from which face, so use normal of grid instead
            if (normal != Vec3f(-2, -2, -2)) mi.setCrossFaceNormal(normal);
            else mi.setCrossFaceNormal(hit.getNormal());
            float t = hit.getT();
            Vec3f intersection = r.pointAtParameter(t);
            int sign_x = r.getDirection().x() >= 0 ? 1 : -1;
            int sign_y = r.getDirection().y() >= 0 ? 1 : -1;
            int sign_z = r.getDirection().z() >= 0 ? 1 : -1;
            mi.setSign(sign_x, sign_y, sign_z);

            Vec3f offset_min = intersection - min;
            Vec3f offset_max = max - intersection;
            float t_delta_x = sign_x == 1 ? offset_max.x() : offset_min.x();
            t_delta_x /= std::abs(r.getDirection().x());
            float t_delta_y = sign_y == 1 ? offset_max.y() : offset_min.y();
            t_delta_y /= std::abs(r.getDirection().y());
            float t_delta_z = sign_z == 1 ? offset_max.z() : offset_min.z();
            t_delta_z /= std::abs(r.getDirection().z());
            mi.setNextT(t + t_delta_x, t + t_delta_y, t + t_delta_z);

            t_delta_x = m_dx / std::abs(r.getDirection().x());
            t_delta_y = m_dy / std::abs(r.getDirection().y());
            t_delta_z = m_dz / std::abs(r.getDirection().z());
            mi.setDeltaT(t_delta_x, t_delta_y, t_delta_z);

            mi.setTmin(t);
        }
        else
        {
            // if a ray intersect with grid bounding box, then it must intersect with at least one voxel
            assert(false);
        }
    }
    
}

std::vector<Vec3f> Grid::getFacePoints(int x, int y, int z, Vec3f normal) const
{
    Vec3f face_center = getVoxelCenter(x, y, z) + normal * Vec3f(m_dx / 2, m_dy / 2, m_dz / 2);
    std::vector<Vec3f> points(4); 
    if (normal.x() != 0)
    {
        points[0] = face_center + Vec3f(0, -m_dy / 2, -m_dz / 2);
        points[1] = face_center + Vec3f(0, m_dy / 2, -m_dz / 2);
        points[2] = face_center + Vec3f(0, m_dy / 2, m_dz / 2);
        points[3] = face_center + Vec3f(0, -m_dy / 2, m_dz / 2);
    }
    else if (normal.y() != 0)
    {
        points[0] = face_center + Vec3f(-m_dx / 2, 0, -m_dz / 2);
        points[1] = face_center + Vec3f(m_dx / 2, 0, -m_dz / 2);
        points[2] = face_center + Vec3f(m_dx / 2, 0, m_dz / 2);
        points[3] = face_center + Vec3f(-m_dx / 2, 0, m_dz / 2);
    }
    else
    {
        points[0] = face_center + Vec3f(-m_dx / 2, -m_dy / 2, 0);
        points[1] = face_center + Vec3f(m_dx / 2, -m_dy / 2, 0);
        points[2] = face_center + Vec3f(m_dx / 2, m_dy / 2, 0);
        points[3] = face_center + Vec3f(-m_dx / 2, m_dy / 2, 0);
    }
    return points;
}
} // namespace object3ds
