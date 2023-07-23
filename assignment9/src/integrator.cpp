#include "integrator.h"
#include "particle.h"
#include "forcefield.h"

void EulerIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt) {
    Vec3f original_position = particle->getPosition();
    particle->setPosition(original_position + particle->getVelocity() * dt);
    particle->setVelocity(particle->getVelocity() + forcefield->getAcceleration(original_position, particle->getMass(), t) * dt);
    particle->increaseAge(dt);
}

void MidpointIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt)
{
    Vec3f mid_position = particle->getPosition() + particle->getVelocity() * (dt / 2);
    Vec3f mid_velocity = particle->getVelocity() + forcefield->getAcceleration(particle->getPosition(), particle->getMass(), t) * (dt / 2);
    particle->setPosition(particle->getPosition() + mid_velocity * dt);
    particle->setVelocity(particle->getVelocity() + forcefield->getAcceleration(mid_position, particle->getMass(), t + dt / 2) * dt);
    particle->increaseAge(dt);
}

void RungeKuttaIntegrator::Update(Particle *particle, ForceField *forcefield, float t, float dt)
{
    Vec3f position = particle->getPosition();
    Vec3f velocity = particle->getVelocity();
    float mass = particle->getMass();
    
    // f is the derivative function
    // k1 = f(tn, yn)
    Vec3f v_k1 = forcefield->getAcceleration(position, mass, t);
    Vec3f p_k1 = velocity;

    // k2 = f(tn + dt / 2, yn + k1 * dt / 2)
    Vec3f v_k2 = forcefield->getAcceleration(position + p_k1 * (dt / 2), mass, t + dt / 2);
    Vec3f p_k2 = velocity + v_k1 * (dt / 2);

    // k3 = f(tn + dt / 2, yn + k2 * dt / 2)
    Vec3f v_k3 = forcefield->getAcceleration(position + p_k2 * (dt / 2), mass, t + dt / 2);
    Vec3f p_k3 = velocity + v_k2 * (dt / 2);

    // k4 = f(tn + dt, yn + k3 * dt)
    Vec3f v_k4 = forcefield->getAcceleration(position + p_k3 * dt, mass, t + dt);
    Vec3f p_k4 = velocity + v_k3 * dt;

    // yn+1 = yn + (k1 + 2 * k2 + 2 * k3 + k4) * dt / 6
    particle->setPosition(position + (p_k1 + p_k2 * 2 + p_k3 * 2 + p_k4) * (dt / 6));
    particle->setVelocity(velocity + (v_k1 + v_k2 * 2 + v_k3 * 2 + v_k4) * (dt / 6));
    particle->increaseAge(dt);
}