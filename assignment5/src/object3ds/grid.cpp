#include "object3ds/grid.h"

namespace object3ds
{
Grid::Grid(std::shared_ptr<BoundingBox> bb, int nx, int ny, int nz) : m_nx(nx), m_ny(ny), m_nz(nz), m_nxs(false, nx), m_nys(false, ny), m_nzs(false, nz)
{
    m_bounding_box = std::shared_ptr<BoundingBox>(bb);
}

bool Grid::intersect(const Ray &r, Hit &h, float tmin)
{
    assert(false);
}

void Grid::paint(void)
{
    assert(false);
}
} // namespace object3ds
