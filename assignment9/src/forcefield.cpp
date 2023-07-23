#include "forcefield.h"

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