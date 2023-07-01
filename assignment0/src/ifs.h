#ifndef _IFS_H_
#define _IFS_H_
#include <vector>
#include <memory>
#include <random>
#include "matrix.h"
#include "image.h"

class IFS {
public:
    IFS() = default;
    ~IFS() = default;

    // read the IFS description from the input file
    void Initialize(const char *filename);

    // render the IFS to the given image
    void Render(Image &image, int num_points, int num_iters) const;

private:
    int m_num_transforms;
    std::vector<std::unique_ptr<Matrix>> m_transform_matrices;
    std::vector<float> m_probabilities;
    
    // used to generate random numbers
    mutable std::discrete_distribution<> m_transform_distribution;
    mutable std::uniform_real_distribution<float> m_points_distribution;
    mutable std::default_random_engine m_random_engine;
};
#endif