#ifndef _FORCEFILED_H_
#define _FORCEFILED_H_
#include "vectors.h"

class ForceField {
public:
    virtual Vec3f getAcceleration(const Vec3f &position, float mass, float t) const = 0;
};

class GravityForceField : public ForceField {
public:
    GravityForceField(const Vec3f &gravity) : m_gravity(gravity) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    Vec3f m_gravity;
};

class ConstantForceField : public ForceField {
public:
    ConstantForceField(const Vec3f &force) : m_force(force) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    Vec3f m_force;
};

class RadialForceField : public ForceField {
public:
    RadialForceField(float magnitude) : m_magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float m_magnitude;
};

class VerticalForceField : public ForceField {
public:
    VerticalForceField(float magnitude) : m_magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float m_magnitude;
};

class WindForceField : public ForceField {
public:
    WindForceField(float magnitude) : m_magnitude(magnitude) {}

    Vec3f getAcceleration(const Vec3f &position, float mass, float t) const override;

private:
    float m_magnitude;
};
#endif