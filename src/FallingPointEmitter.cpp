/// Spells - a game about magic spells
/// Copyright (C) 2015 - 2018  Foaly

/// This program is free software: you can redistribute it and/or modify
/// it under the terms of the GNU General Public License as published by
/// the Free Software Foundation, either version 3 of the License, or
/// (at your option) any later version.

/// This program is distributed in the hope that it will be useful,
/// but WITHOUT ANY WARRANTY; without even the implied warranty of
/// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
/// GNU General Public License for more details.

/// You should have received a copy of the GNU General Public License
/// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// File created by Maximilian on 30.07.2015.


#include <Thor/Math/Random.hpp>

#include "FallingPointEmitter.hpp"

FallingPointEmitter::FallingPointEmitter() :
    m_color(165, 0, 0, 200) // default: slightly transparent red
{
    
}

void FallingPointEmitter::operator()(thor::EmissionInterface &system, sf::Time dt)
{
    for(auto iter = m_positions.begin(); iter != m_positions.end();)
    {
        thor::Particle particle(sf::seconds(5.f));
        particle.position = *(iter);
        particle.velocity = sf::Vector2f(0.f, thor::random(300.f, 400.f));
        particle.scale = sf::Vector2f(0.8f, 0.8f);
        particle.color = m_color;

        system.emitParticle(particle);

        iter = m_positions.erase(iter);
    }

}

void FallingPointEmitter::emitParticle(sf::Vector2f position)
{
    m_positions.push_back(position);

}

void FallingPointEmitter::setColor(sf::Color color)
{
    m_color = color;
}
