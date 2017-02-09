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

// Created by Maximilian on 26.07.2015.


#ifndef SPELLS_VECTOREMITTER_H
#define SPELLS_VECTOREMITTER_H

#include <Thor/Math/Distribution.hpp>
#include <Thor/Particles/EmissionInterface.hpp>

#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <vector>

/// @brief Class that emits particles with customizable initial conditions.
/// @details This emitter is universal with respect to the initial conditions of each emitted particle. It works with callbacks
///  that return initial values for the particle attributes (position, rotation, color, ...). So you can pass constants, random
///  distributions, or any functions that compute the value in an arbitrary way. Have a look at the thor::Distributions namespace
///  for useful predefined functions.
class  VectorEmitter
{
    // ---------------------------------------------------------------------------------------------------------------------------
    // Public member functions
public:
    /// @brief Default constructor
    ///
    VectorEmitter(std::vector<sf::Vector2f>& points);

    /// @brief Emits particles into a particle system.
    /// @param system Indirection to the particle system that stores the particles.
    /// @param dt Time interval during which particles are emitted.
    void                        operator() (thor::EmissionInterface& system, sf::Time dt);

    /// @brief Sets the particle emission rate.
    /// @param particlesPerSecond How many particles are emitted in 1 second. The type is not integral to allow
    ///  more flexibility (e.g. 0.5 yields one particle every 2 seconds).
    void                        setEmissionRate(float particlesPerSecond);

    /// @brief Sets the lifetime (time between emission and death) of the particle.
    /// @param particleLifetime Constant or function returning the particle lifetime.
    void                        setParticleLifetime(thor::Distribution<sf::Time> particleLifetime);

    /// @brief Sets the initial particle position.
    /// @param particlePosition Constant or function returning the initial particle position.
    void                        setParticlePosition(thor::Distribution<sf::Vector2f> particlePosition);

    /// @brief Sets the initial particle velocity.
    /// @param particleVelocity Constant or function returning the initial particle velocity.
    void                        setParticleVelocity(thor::Distribution<sf::Vector2f> particleVelocity);

    /// @brief Sets the initial particle rotation.
    /// @param particleRotation Constant or function returning the initial particle rotation.
    void                        setParticleRotation(thor::Distribution<float> particleRotation);

    /// @brief Sets the initial particle rotation speed.
    /// @param particleRotationSpeed Constant or function returning the initial particle rotation speed.
    void                        setParticleRotationSpeed(thor::Distribution<float> particleRotationSpeed);

    /// @brief Sets the initial particle scale.
    /// @param particleScale Constant or function returning the initial particle scale.
    void                        setParticleScale(thor::Distribution<sf::Vector2f> particleScale);

    /// @brief Sets the initial particle color.
    /// @param particleColor Constant or function returning the initial particle color.
    void                        setParticleColor(thor::Distribution<sf::Color> particleColor);

    /// @brief Sets the initial particle texture index.
    /// @param particleTextureIndex Constant or function returning the initial index of the particle's texture rectangle.
    void                        setParticleTextureIndex(thor::Distribution<unsigned int> particleTextureIndex);


    // ---------------------------------------------------------------------------------------------------------------------------
    // Private member functions
    private:
    // Returns the number of particles to emit during this frame.
    std::size_t                    computeParticleCount(sf::Time dt);

    void                           emitParticle(thor::EmissionInterface& system, sf::Vector2f position);


    // ---------------------------------------------------------------------------------------------------------------------------
    // Private variables
private:
    std::vector<sf::Vector2f>&          m_points;

    bool                                m_firstEmission;
    float                               mEmissionRate;
    float                               mEmissionDifference;

    thor::Distribution<sf::Time>        mParticleLifetime;
    thor::Distribution<sf::Vector2f>    mParticlePosition;
    thor::Distribution<sf::Vector2f>    mParticleVelocity;
    thor::Distribution<float>           mParticleRotation;
    thor::Distribution<float>           mParticleRotationSpeed;
    thor::Distribution<sf::Vector2f>    mParticleScale;
    thor::Distribution<sf::Color>       mParticleColor;
    thor::Distribution<unsigned int>    mParticleTextureIndex;
};


#endif //SPELLS_VECTOREMITTER_H
