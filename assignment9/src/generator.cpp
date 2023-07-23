#include "generator.h"
#include "vectors.h"
#include "particle.h"
#include <GL/gl.h>
constexpr float ground_y = -3.4f;

Particle* HoseGenerator::Generate(float current_time, int i)
{
    Vec3f position = m_position;
    if (m_position_randomness > 0)
    {
        position += m_random.randomVector() * m_position_randomness;
    }
    
    Vec3f velocity = m_velocity;
    if (m_velocity_randomness > 0)
    {
        velocity += m_random.randomVector() * m_velocity_randomness;
    }

    return new Particle(position, velocity, m_color, m_dead_color, m_mass, m_lifespan);
}

Particle* RingGenerator::Generate(float current_time, int i)
{
    Vec3f position = m_random.randomVector();
    position.Set(position.x(), 0, position.z());
    position.Normalize(); // direction
    position *= current_time / m_lifespan;
    if (m_position_randomness > 0)
    {
        position += m_random.randomVector() * m_position_randomness;
    }
    position.Set(position.x(), ground_y, position.z());

    Vec3f velocity = m_velocity;
    if (m_velocity_randomness > 0)
    {
        velocity += m_random.randomVector() * m_velocity_randomness;
    }

    return new Particle(position, velocity, m_color, m_dead_color, m_mass, m_lifespan);
}

int RingGenerator::numNewParticles(float current_time, float dt) const
{
    return Generator::numNewParticles(current_time, dt) * current_time;
}

void RingGenerator::Paint() const
{
    // draw a polygon to represent the ground plane
    glDisable(GL_LIGHTING);
    glBegin(GL_QUADS);
    glColor3f(0.27f, 0.27f, 0.27f);
    glVertex3f(-7.0f, ground_y, -5.0f);
    glVertex3f(-7.0f, ground_y, 20.0f);
    glVertex3f(7.0f, ground_y, 20.0f);
    glVertex3f(7.0f, ground_y, -5.0f);
    glEnd();
}
