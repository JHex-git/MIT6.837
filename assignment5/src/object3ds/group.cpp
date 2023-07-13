#include "object3ds/group.h"
#include "object3ds/boundingbox.h"

namespace object3ds
{
Group::Group(int num_objects) : Object3D(nullptr)
{
    m_objects = std::vector<std::shared_ptr<Object3D>>(num_objects);
    if (num_objects > 0)
    {
        int i = 0;
        while (i < num_objects && m_objects[i]->getBoundingBox() == nullptr)
        {
            ++i;
        }

        if (i < num_objects)
        {
            m_bounding_box = std::make_shared<BoundingBox>(m_objects[i]->getBoundingBox()->getMin(), m_objects[i]->getBoundingBox()->getMax());
            ++i;
            for (; i < num_objects; ++i)
            {
                m_bounding_box->Extend(m_objects[i]->getBoundingBox().get());
            }
        }
        else
        {
            m_bounding_box = nullptr;
        }
    }
    else
    {
        m_bounding_box = nullptr;
    }
}

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

void Group::insertIntoGrid(Grid *g, Matrix *m)
{
    for (auto obj: m_objects)
    {
        obj->insertIntoGrid(g, m);
    }
}
} // namespace object3ds
