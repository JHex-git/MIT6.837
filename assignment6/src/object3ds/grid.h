#ifndef _GRID_H_
#define _GRID_H_
#include "object3ds/object3d.h"
#include "raytrace/marching_info.h"
#include "raytrace/rayTree.h"
#include <vector>

namespace utility
{
    class Vec3f;
}

namespace object3ds
{
using utility::Vec3f;
using raytrace::MarchingInfo;
using raytrace::RayTree;

class Grid: public Object3D
{
public:
    Grid(std::shared_ptr<BoundingBox> bb, int nx, int ny, int nz, bool visualize_grid);
    ~Grid();
    
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint(void) const override;

    inline void addObjectToVoxel(int x, int y, int z, std::shared_ptr<Object3D> obj) { m_voxel_objects[x][y][z].push_back(obj); }
    inline const std::vector<std::shared_ptr<Object3D>>& getObjectsInVoxel(int x, int y, int z) const { return m_voxel_objects[x][y][z]; }
    inline void addNonVoxelObject(std::shared_ptr<Object3D> obj) { m_non_voxel_objects.push_back(obj); }
    
    inline float getVoxelDiagonalLength() const { return std::sqrt(m_dx * m_dx + m_dy * m_dy + m_dz * m_dz); }
    Vec3f getVoxelCenter(int x, int y, int z) const;
    // for pointer outside the bounding box, return (-1, -1, -1)
    std::vector<int> getVoxelIndex(const Vec3f &p) const;
    inline std::vector<int> getVoxelNum() const { return {m_nx, m_ny, m_nz}; }

private:
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;
    bool intersectBox(const Ray &r, Hit &h, Vec3f min, Vec3f max, float tmin) const;
    std::vector<Vec3f> getFacePoints(int x, int y, int z, Vec3f normal) const;

    int m_nx, m_ny, m_nz;
    float m_dx, m_dy, m_dz;
    bool m_visualize_grid;
    std::vector<std::vector<std::vector<std::vector<std::shared_ptr<Object3D>>>>> m_voxel_objects;
    std::vector<std::shared_ptr<Object3D>> m_non_voxel_objects;
    MarchingInfo m_mi;

    Material* cell_materials[RayTree::color_gradient_num];
};
} // namespace object3ds

#endif