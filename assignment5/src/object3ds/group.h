#ifndef _GROUP_H_
#define _GROUP_H_

#include "object3ds/object3d.h"
#include <vector>
#include <memory>

namespace object3ds
{
using raytrace::Hit;
using raytrace::Ray;

class Group : public Object3D
{
public:
    Group(int num_objects);

    inline void addObject(int index, Object3D* obj)
    {
        m_objects[index] = std::shared_ptr<Object3D>(obj);
    }

    bool intersect(const Ray &r, Hit &h, float tmin) override;
    void paint(void) override;

private:
    std::vector<std::shared_ptr<Object3D>> m_objects;
};  
} // namespace object3ds


#endif