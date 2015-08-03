//
// Created by Maximilian on 30.07.2015.
//

#ifndef SPELLS_FALLINGPOINTEMITTER_H
#define SPELLS_FALLINGPOINTEMITTER_H

#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Particles/ParticleSystem.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

class FallingPointEmitter
{
public:
    void operator() (thor::EmissionInterface& system, sf::Time dt);

    void emitParticle(sf::Vector2f position);

private:
    std::vector<sf::Vector2f> m_positions;
};


#endif //SPELLS_FALLINGPOINTEMITTER_H
