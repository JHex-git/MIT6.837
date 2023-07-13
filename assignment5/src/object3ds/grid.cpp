#include "object3ds/grid.h"
#include "object3ds/boundingbox.h"
#include "utility/vectors.h"
#include <GL/gl.h>

namespace object3ds
{

Grid::Grid(std::shared_ptr<BoundingBox> bb, int nx, int ny, int nz) : m_nx(nx), m_ny(ny), m_nz(nz), m_voxels(nx, std::vector<std::vector<bool>>(ny, std::vector<bool>(nz, false)))
{
    m_bounding_box = bb;
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

void Grid::paint(void) const
{
    glBegin(GL_QUADS);
    for (int i = 0; i < m_nx; ++i)
    {
        for (int j = 0; j < m_ny; ++j)
        {
            for (int k = 0; k < m_nz; ++k)
            {
                if (m_voxels[i][j][k])
                {
                    Vec3f center = getVoxelCenter(i, j, k);
                    Vec3f normal;
                    Vec3f p1, p2, p3, p4;
                    
                    // bottom
                    p1 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    p2 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    p3 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    p4 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    normal = Vec3f(0, 0, -1);
                    glNormal3f(normal[0], normal[1], normal[2]);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glVertex3f(p3[0], p3[1], p3[2]);
                    glVertex3f(p4[0], p4[1], p4[2]);

                    // top
                    p1 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    p2 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    p3 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    p4 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    normal = Vec3f(0, 0, 1);
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
                    glNormal3f(normal[0], normal[1], normal[2]);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glVertex3f(p3[0], p3[1], p3[2]);
                    glVertex3f(p4[0], p4[1], p4[2]);

                    // front
                    p1 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    p2 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() - m_dz / 2);
                    p3 = Vec3f(center.x() + m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    p4 = Vec3f(center.x() - m_dx / 2, center.y() - m_dy / 2, center.z() + m_dz / 2);
                    normal = Vec3f(0, -1, 0);
                    glNormal3f(normal[0], normal[1], normal[2]);
                    glVertex3f(p1[0], p1[1], p1[2]);
                    glVertex3f(p2[0], p2[1], p2[2]);
                    glVertex3f(p3[0], p3[1], p3[2]);
                    glVertex3f(p4[0], p4[1], p4[2]);

                    // back
                    p1 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    p2 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() - m_dz / 2);
                    p3 = Vec3f(center.x() + m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    p4 = Vec3f(center.x() - m_dx / 2, center.y() + m_dy / 2, center.z() + m_dz / 2);
                    normal = Vec3f(0, 1, 0);
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
    int x = (p.x() - min.x()) / m_dx;
    int y = (p.y() - min.y()) / m_dy;
    int z = (p.z() - min.z()) / m_dz;
    return std::vector<int>{x, y, z};
}
} // namespace object3ds
