#ifndef _GRID_H_
#define _GRID_H_
#include "object3ds/object3d.h"
#include <vector>


namespace object3ds
{

class Grid: public Object3D
{
public:
    Grid(std::shared_ptr<BoundingBox> bb, int nx, int ny, int nz); 
    
    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint(void) override;

private:
    int m_nx, m_ny, m_nz;
    std::vector<bool> m_nxs;
    std::vector<bool> m_nys;
    std::vector<bool> m_nzs;
    std::shared_ptr<BoundingBox> m_bounding_box;
};
} // namespace object3ds

#endif