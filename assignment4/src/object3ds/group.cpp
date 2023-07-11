#include "object3ds/group.h"

namespace object3ds
{
bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
    bool exist_intersect = false;
    Hit tmp;
    float t_closest = std::numeric_limits<float>::max();
    for (auto obj: m_objects)
    {
        bool tmp_intersect = false;
        tmp_intersect = obj->intersect(r, tmp, tmin);

        if (tmp_intersect && tmp.getT() < t_closest)
        {
            exist_intersect = true;
            t_closest = tmp.getT(); // get the intersection closest to ray origin
            h = tmp;
        }
    }
    return exist_intersect;
}

void Group::paint(void)
{
    for (auto obj: m_objects)
    {
        obj->paint();
    }
}
} // namespace object3ds
