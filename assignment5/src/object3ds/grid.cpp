#include "object3ds/grid.h"
#include "object3ds/boundingbox.h"
#include "utility/vectors.h"

namespace object3ds
{
    
Grid::Grid(std::shared_ptr<BoundingBox> bb, int nx, int ny, int nz) : m_nx(nx), m_ny(ny), m_nz(nz), m_voxels(nx, std::vector<std::vector<bool>>(ny, std::vector<bool>(nz, false)))
{
    m_bounding_box = std::shared_ptr<BoundingBox>(bb);
    Vec3f min = m_bounding_box->getMin();
    Vec3f max = m_bounding_box->getMax();
    m_dx = (max.x() - min.x()) / m_nx;
    m_dy = (max.y() - min.y()) / m_ny;
    m_dz = (max.z() - min.z()) / m_nz;
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{
    assert(false);
}

void Grid::paint(void)
{
    assert(false);
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
    int x = (p.x() - min.x()) / m_dx;
    int y = (p.y() - min.y()) / m_dy;
    int z = (p.z() - min.z()) / m_dz;
    return std::vector<int>{x, y, z};
}
} // namespace object3ds
