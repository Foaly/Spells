//
// Created by Maximilian on 30.07.2015.
//

#include <Thor/Math/Random.hpp>

#include "FallingPointEmitter.hpp"

void FallingPointEmitter::operator()(thor::EmissionInterface &system, sf::Time dt)
{
    for(auto iter = m_positions.begin(); iter != m_positions.end();)
    {
        thor::Particle particle(sf::seconds(5.f));
        particle.position = *(iter);
        particle.velocity = sf::Vector2f(0.f, thor::random(300.f, 400.f));
        particle.scale = sf::Vector2f(0.8f, 0.8f);
        particle.color = sf::Color(165, 0, 0, 200);

        system.emitParticle(particle);

        iter = m_positions.erase(iter);
    }

}

void FallingPointEmitter::emitParticle(sf::Vector2f position)
{
    m_positions.push_back(position);

}
