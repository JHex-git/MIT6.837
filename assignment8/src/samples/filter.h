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
    Filter(float support_radius) : m_support_radius(support_radius + 0.49999) {}

    Vec3f getColor(int i, int j, Film *film);

    int getSupportRadius() { return m_support_radius; }

    // x and y are relative to (0, 0)
    virtual float getWeight(float x, float y) = 0;

private:
    int m_support_radius;
};

class BoxFilter : public Filter
{
public:
    BoxFilter(float radius) : Filter(radius), m_radius(radius) {}

    float getWeight(float x, float y) override;

private:
    float m_radius;
};

class TentFilter : public Filter
{
public:
    TentFilter(float radius) : Filter(radius), m_radius(radius) {}

    float getWeight(float x, float y) override;

private:
    float m_radius;
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