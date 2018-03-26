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

// File created by Maximilian on 25.03.2018


#ifndef SPELLS_AFFECTORS_H
#define SPELLS_AFFECTORS_H

#include <Thor/Particles/Particle.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>

#include <cmath>

#include "Util.hpp"


class DirectedAffector
{
public:
    explicit DirectedAffector(sf::Vector2f target) : m_target(target) {}
    void operator() (thor::Particle& particle, sf::Time dt)
    {
        sf::Vector2f direction = m_target - particle.position;
        auto u = thor::unitVector(direction);
        particle.velocity = u * thor::length(direction) * std::sqrt(thor::getRemainingRatio(particle) + 0.5f);
        particle.rotation = std::atan2(m_target.y - particle.position.y, m_target.x - particle.position.x) * 180 / thor::Pi;
    }

private:
    sf::Vector2f    m_target;
};


class ScaleXUpDownAffector
{
public:
    void operator() (thor::Particle& particle, sf::Time dt)
    {
        float scale = 1.f;
        float ratio = thor::getRemainingRatio(particle);
        if (ratio <= 0.3f)
            scale = util::linearInterpolation(0.1f, 1.f, ratio * 3.3333333333333333333f);
        else if (ratio >= 0.8)
            scale = util::linearInterpolation(1.f, 0.1f, (ratio - 0.8f) * 5.f);

        scale *= util::sign(particle.scale.x);
        particle.scale = sf::Vector2f(scale, particle.scale.y);
    }
};


/// @brief Scales particles over time. This time respecting the sign of the scale
class ScaleAffector
{
public:
    /// @brief Constructor
    /// @param scaleFactor Factor by which particles are scaled every second.
    explicit    ScaleAffector(sf::Vector2f scaleFactor) : m_scaleFactor(scaleFactor) {}

    void    operator() (thor::Particle& particle, sf::Time dt)
    {
        sf::Vector2f sign(util::sign(particle.scale.x), util::sign(particle.scale.y));
        particle.scale += dt.asSeconds() * m_scaleFactor;
        particle.scale = thor::cwiseProduct(particle.scale, sign);
    }

private:
    sf::Vector2f    m_scaleFactor;
};

#endif //SPELLS_AFFECTORS_H
