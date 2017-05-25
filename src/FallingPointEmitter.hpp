/// Spells - a game about magic spells
/// Copyright (C) 2015 - 2017  Foaly

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


#ifndef SPELLS_FALLINGPOINTEMITTER_H
#define SPELLS_FALLINGPOINTEMITTER_H

#include <Thor/Particles/EmissionInterface.hpp>
#include <Thor/Particles/ParticleSystem.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

class FallingPointEmitter
{
public:
    FallingPointEmitter();
    
    
    void operator() (thor::EmissionInterface& system, sf::Time dt);

    void emitParticle(sf::Vector2f position);
    
    void setColor(sf::Color color);

private:
    std::vector<sf::Vector2f> m_positions;
    sf::Color                 m_color;
};


#endif //SPELLS_FALLINGPOINTEMITTER_H
