#ifndef _FILTER_H_
#define _FILTER_H_

namespace utility
{
    class Vec3f;
} // namespace utility

namespace samples
{
class Film;

using utility::Vec3f;

class Filter
{
public:
    Filter(int support_radius) : m_support_radius(support_radius) {}

    Vec3f getColor(int i, int j, Film *film);

    int getSupportRadius() { return m_support_radius; }

    // x and y are in the range [-0.5, 0.5]
    virtual float getWeight(float x, float y) = 0;

private:
    int m_support_radius;
};

class BoxFilter : public Filter
{
public:
    BoxFilter(int support_radius) : Filter(support_radius) {}

    float getWeight(float x, float y) override;
};

class TentFilter : public Filter
{
public:
    TentFilter(int support_radius) : Filter(support_radius) {}

    float getWeight(float x, float y) override;
};

class GaussianFilter : public Filter
{
public:
    GaussianFilter(float sigma) : Filter((int)(2 * sigma)), m_sigma(sigma) {}

    float getWeight(float x, float y) override;
private:
    float m_sigma;
};
} // namespace samples

#endif