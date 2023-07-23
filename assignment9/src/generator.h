#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include "vectors.h"
#include "random.h"

class Particle;

class Generator
{
public:
  // initialization
  void SetColors(Vec3f color, Vec3f dead_color, float color_randomness)
  {
    m_color = color;
    m_dead_color = dead_color;
    m_color_randomness = color_randomness;
  }

  void SetLifespan(float lifespan, float lifespan_randomness, int desired_num_particles)
  {
    m_lifespan = lifespan;
    m_lifespan_randomness = lifespan_randomness;
    m_desired_num_particles = desired_num_particles;
  }

  void SetMass(float mass, float mass_randomness)
  {
    m_mass = mass;
    m_mass_randomness = mass_randomness;
  }

  // on each timestep, create some particles
  virtual int numNewParticles(float current_time, float dt) const
  {
    return std::max((int)(dt * m_desired_num_particles / m_lifespan), 1);
  }

  virtual Particle* Generate(float current_time, int i) = 0;

  // for the gui
  virtual void Paint() const {}

  void Restart()
  {
    m_random = Random(); // reset the random number generator
  }

protected:
  Vec3f m_color;
  Vec3f m_dead_color;
  float m_color_randomness;

  float m_lifespan;
  float m_lifespan_randomness;
  int m_desired_num_particles;

  float m_mass;
  float m_mass_randomness;

  Random m_random;
};

class HoseGenerator : public Generator
{
public:
  HoseGenerator(Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness) : 
                m_position(position), m_position_randomness(position_randomness), 
                m_velocity(velocity), m_velocity_randomness(velocity_randomness) {}

  Particle* Generate(float current_time, int i) override;

private:
  Vec3f m_position;
  float m_position_randomness;
  Vec3f m_velocity;
  float m_velocity_randomness;
};

class RingGenerator : public Generator
{
public:
  RingGenerator(float position_randomness, Vec3f velocity, float velocity_randomness) : 
                m_position_randomness(position_randomness), m_velocity(velocity), m_velocity_randomness(velocity_randomness) {}

  Particle* Generate(float current_time, int i) override;

  int numNewParticles(float current_time, float dt) const override;

  void Paint() const;

private:
  float m_position_randomness;
  Vec3f m_velocity;
  float m_velocity_randomness;
};
#endif