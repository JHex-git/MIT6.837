#ifndef _SAMPLER_H_
#define _SAMPLER_H_

namespace utility
{
    class Vec2f;
} // namespace utility

namespace samples
{
using utility::Vec2f;

class Sampler
{
public:
    Sampler(int samples_per_pixel) : m_samples_per_pixel(samples_per_pixel) {}

    virtual Vec2f getSamplePosition(int n) = 0;

    // ACCESSORS
    int getNumSamplesPerPixel() const { return m_samples_per_pixel; }

protected:
    int m_samples_per_pixel;
};
} // namespace samples


#endif