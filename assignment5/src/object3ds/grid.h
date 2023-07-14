#ifndef _GRID_H_
#define _GRID_H_
#include "object3ds/object3d.h"
#include "raytrace/marching_info.h"
#include <vector>

namespace utility
{
    class Vec3f;
}

namespace object3ds
{
using utility::Vec3f;
using raytrace::MarchingInfo;

class Grid: public Object3D
{
public:
    Grid(std::shared_ptr<BoundingBox> bb, int nx, int ny, int nz); 
    
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint(void) const override;

    inline bool getVoxel(int x, int y, int z) const { return m_voxels[x][y][z]; }
    inline void setVoxel(int x, int y, int z, bool opaque) { m_voxels[x][y][z] = opaque; }
    inline float getVoxelDiagonalLength() const { return std::sqrt(m_dx * m_dx + m_dy * m_dy + m_dz * m_dz); }
    Vec3f getVoxelCenter(int x, int y, int z) const;
    // for pointer outside the bounding box, return (-1, -1, -1)
    std::vector<int> getVoxelIndex(const Vec3f &p) const;

private:
    void initializeRayMarch(MarchingInfo &mi, const Ray &r, float tmin) const;
    bool intersectBox(const Ray &r, Hit &h, Vec3f min, Vec3f max, float tmin) const;
    std::vector<Vec3f> getFacePoints(int x, int y, int z, Vec3f normal) const;

    int m_nx, m_ny, m_nz;
    float m_dx, m_dy, m_dz;
    std::vector<std::vector<std::vector<bool>>> m_voxels; // true if occupied
    MarchingInfo m_mi;
};
} // namespace object3ds

#endif