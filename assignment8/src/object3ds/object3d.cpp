#include "object3ds/object3d.h"
#include "object3ds/boundingbox.h"
#include "object3ds/grid.h"
#include "object3ds/transform.h"
#include "utility/matrix.h"

namespace object3ds
{

void Object3D::insertIntoGrid(Grid *g, Matrix *m)
{
    constexpr float epsilon = 0.0001;
    auto min = m_bounding_box->getMin();
    auto max = m_bounding_box->getMax();
    if (m != nullptr)
    {
        auto p1 = Vec3f(min[0], min[1], min[2]), p2 = Vec3f(min[0], min[1], max[2]), 
             p3 = Vec3f(min[0], max[1], min[2]), p4 = Vec3f(min[0], max[1], max[2]),
             p5 = Vec3f(max[0], min[1], min[2]), p6 = Vec3f(max[0], min[1], max[2]), 
             p7 = Vec3f(max[0], max[1], min[2]), p8 = Vec3f(max[0], max[1], max[2]);
        m->Transform(p1), m->Transform(p2), m->Transform(p3), m->Transform(p4),
        m->Transform(p5), m->Transform(p6), m->Transform(p7), m->Transform(p8);

        min = Vec3f(std::min(std::min(std::min(std::min(std::min(std::min(std::min(p1[0], p2[0]), p3[0]), p4[0]), p5[0]), p6[0]), p7[0]), p8[0]),
              std::min(std::min(std::min(std::min(std::min(std::min(std::min(p1[1], p2[1]), p3[1]), p4[1]), p5[1]), p6[1]), p7[1]), p8[1]),
              std::min(std::min(std::min(std::min(std::min(std::min(std::min(p1[2], p2[2]), p3[2]), p4[2]), p5[2]), p6[2]), p7[2]), p8[2]));
        max = Vec3f(std::max(std::max(std::max(std::max(std::max(std::max(std::max(p1[0], p2[0]), p3[0]), p4[0]), p5[0]), p6[0]), p7[0]), p8[0]),
                std::max(std::max(std::max(std::max(std::max(std::max(std::max(p1[1], p2[1]), p3[1]), p4[1]), p5[1]), p6[1]), p7[1]), p8[1]),
                std::max(std::max(std::max(std::max(std::max(std::max(std::max(p1[2], p2[2]), p3[2]), p4[2]), p5[2]), p6[2]), p7[2]), p8[2]));
    }

    // When the bounding box is on the edge of a voxel, the voxel will be missed, so extra epsilon is needed.
    auto min_index = g->getVoxelIndex(min - Vec3f(epsilon, epsilon, epsilon));
    for (int i = 0; i < min_index.size(); ++i)
    {
        min_index[i] = std::max(min_index[i], 0);
    }

    // When the bounding box is on the edge of a voxel, the voxel will be missed, so extra voxels are needed.
    auto max_index = g->getVoxelIndex(max + Vec3f(epsilon, epsilon, epsilon));
    auto voxel_num = g->getVoxelNum();
    for (int i = 0; i < max_index.size(); ++i)
    {
        max_index[i] = max_index[i] == -1 ? voxel_num[i] - 1 : max_index[i];
    }

    std::shared_ptr<Object3D> transform_ptr = nullptr;
    if (m != nullptr) transform_ptr = std::make_shared<Transform>(*m, this);
    else transform_ptr = std::shared_ptr<Object3D>(this);
    for (int i = min_index[0]; i <= max_index[0]; i++)
    {
        for (int j = min_index[1]; j <= max_index[1]; j++)
        {
            for (int k = min_index[2]; k <= max_index[2]; k++)
            {
                // TODO: potential bug: repeated insertion
                g->addObjectToVoxel(i, j, k, transform_ptr);
            }
        }
    }
}
} // namespace object3ds
