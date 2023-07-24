#include "forcefield.h"
#include "perlin_noise.h"

Vec3f GravityForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return m_gravity;
}

Vec3f ConstantForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return m_force * (1 / mass);
}

Vec3f RadialForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    Vec3f direction = position;
    direction.Negate();
    direction.Normalize();
    return position.Length() * m_magnitude / mass * direction;
}

Vec3f VerticalForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    return position.y() * m_magnitude / mass * Vec3f(0, -1, 0);
}

Vec3f WindForceField::getAcceleration(const Vec3f &position, float mass, float t) const {
    float noise_z = PerlinNoise::noise(position.x(), position.y(), t);
    float noise_x = PerlinNoise::noise(t, position.y(), position.z());
    // float noise_y = PerlinNoise::noise(position.x(), t, position.z()) + 0.5;
    Vec3f force = Vec3f(noise_x, 0, noise_z);
    force.Normalize();
    return force * (m_magnitude / mass);
}