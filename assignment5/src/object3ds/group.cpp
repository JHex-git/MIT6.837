#include "object3ds/group.h"
#include "object3ds/boundingbox.h"

namespace object3ds
{
Group::Group(int num_objects) : Object3D(nullptr)
{
    m_objects = std::vector<std::shared_ptr<Object3D>>(num_objects);
    m_bounding_box = nullptr;
}

void Group::addObject(int index, Object3D* obj)
{
    assert(index < m_objects.size());
    assert(obj != nullptr);
    assert(m_objects[index] == nullptr);

    m_objects[index] = std::shared_ptr<Object3D>(obj);
    if (m_bounding_box == nullptr) m_bounding_box = std::make_shared<BoundingBox>(m_objects[index]->getBoundingBox()->getMin(), m_objects[index]->getBoundingBox()->getMax());
    else 
    {
        BoundingBox* bb = m_objects[index]->getBoundingBox().get();
        if (bb != nullptr) m_bounding_box->Extend(m_objects[index]->getBoundingBox().get());
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

void Group::paint(void) const
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
