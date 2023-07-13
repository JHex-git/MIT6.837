#ifndef _GRID_H_
#define _GRID_H_
#include "object3ds/object3d.h"
#include <vector>

namespace utility
{
    class Vec3f;
}

namespace object3ds
{
using utility::Vec3f;

class Grid: public Object3D
{
public:
    Grid(std::shared_ptr<BoundingBox> bb, int nx, int ny, int nz); 
    
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint(void) const override;

    inline bool getVoxel(int x, int y, int z) const { return m_voxels[x][y][z]; }
    inline void setVoxel(int x, int y, int z, bool opaque) { m_voxels[x][y][z] = opaque; }
    Vec3f getVoxelCenter(int x, int y, int z) const;
    std::vector<int> getVoxelIndex(const Vec3f &p) const;

private:
    int m_nx, m_ny, m_nz;
    float m_dx, m_dy, m_dz;
    std::vector<std::vector<std::vector<bool>>> m_voxels; // true if occupied
};
} // namespace object3ds

#endif