#ifndef _INTEGRATOR_H_
#define _INTEGRATOR_H_
#include "vectors.h"

class Particle;
class ForceField;

class Integrator
{
public:
    Integrator(Vec3f color) : m_visualize_color(color) {}

    virtual void Update(Particle *particle, ForceField *forcefield, float t, float dt) = 0;
    Vec3f getColor() const { return m_visualize_color; }

private:
    Vec3f m_visualize_color;
};

class EulerIntegrator : public Integrator
{
public:
    EulerIntegrator() : Integrator(Vec3f(1, 0, 0)) {}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;
};

class MidpointIntegrator : public Integrator
{
public:
    MidpointIntegrator() : Integrator(Vec3f(0, 1, 0)) {}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;
};

class RungeKuttaIntegrator : public Integrator
{
public:
    RungeKuttaIntegrator() : Integrator(Vec3f(0, 0, 1)) {}

    void Update(Particle *particle, ForceField *forcefield, float t, float dt) override;
};
#endif