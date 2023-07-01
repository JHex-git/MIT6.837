#include "ifs.h"
#include <fstream>

void IFS::Initialize(const char *filename) {
    std::ifstream input(filename);
    if (!input.is_open()) {
        std::cerr << "Error: could not open file " << filename << std::endl;
        exit(1);
    }

    input >> m_num_transforms;
    m_transform_matrices.resize(m_num_transforms);
    m_probabilities.resize(m_num_transforms);
    for (int i = 0; i < m_num_transforms; ++i)
    {
        m_transform_matrices[i] = std::make_unique<Matrix>();
        input >> m_probabilities[i];
        for (int j = 0; j < 9; ++j)
        {
            float transform_item;
            input >> transform_item;
            m_transform_matrices[i]->Set(j % 3, j / 3, transform_item);
        }
    }
    m_transform_distribution = std::discrete_distribution<>(m_probabilities.begin(), m_probabilities.end());

    input.close();
}

void IFS::Render(Image &image, int num_points, int num_iters) const {
    // set background to white
    image.SetAllPixels(Vec3f(1, 1, 1));

    for (int i = 0; i < num_points; ++i)
    {
        m_points_distribution(m_random_engine);
        Vec3f point = Vec3f(m_points_distribution(m_random_engine), m_points_distribution(m_random_engine), 1);
        for (int k = 0; k < num_iters; ++k)
        {
            m_transform_matrices[m_transform_distribution(m_random_engine)]->Transform(point);
        }
        point /= point.z();
        int x = point.x() * image.Width();
        int y = point.y() * image.Height();
        image.SetPixel(x, y, Vec3f(0, 0, 0));
    }
}