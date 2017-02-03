//
// Created by Maximilian on 26.07.2015.
//
#include <Thor/Particles/Particle.hpp>
#include <Thor/Math/Random.hpp>

#include "VectorEmitter.hpp"

VectorEmitter::VectorEmitter(std::vector<sf::Vector2f>& points)
        : m_points(points)
        , m_firstEmission(true)
        , mEmissionRate(1.f)
        , mEmissionDifference(0.f)
        , mParticleLifetime(sf::seconds(1.f))
        , mParticlePosition(sf::Vector2f(0.f, 0.f))
        , mParticleVelocity(sf::Vector2f(0.f, 0.f))
        , mParticleRotation(0.f)
        , mParticleRotationSpeed(0.f)
        , mParticleScale(sf::Vector2f(1.f, 1.f))
        , mParticleColor(sf::Color::White)
        , mParticleTextureIndex(0u)
{
}

void VectorEmitter::operator() (thor::EmissionInterface& system, sf::Time dt)
{
    if(m_firstEmission)
    {
        for(auto itor = m_points.begin(); itor != m_points.end(); itor++)
        {
            emitParticle(system, *itor);
        }

        m_firstEmission = false;
    }
    else
    {
        const std::size_t nbParticles = computeParticleCount(dt);

        for (std::size_t i = 0; i < nbParticles; ++i)
        {
            const unsigned int randomIndex= thor::random(0u, m_points.size() - 1u);
            emitParticle(system, m_points[randomIndex]);
        }
    }
}

void VectorEmitter::emitParticle(thor::EmissionInterface& system, sf::Vector2f position)
{
    // Create particle and specify parameters
    thor::Particle particle( mParticleLifetime() );
    particle.position = position;
    particle.velocity = mParticleVelocity();
    particle.rotation = mParticleRotation();
    particle.rotationSpeed = mParticleRotationSpeed();
    particle.scale = mParticleScale();
    particle.color = mParticleColor();
    particle.textureIndex = mParticleTextureIndex();

    system.emitParticle(particle);
}


void VectorEmitter::setEmissionRate(float particlesPerSecond)
{
    mEmissionRate = particlesPerSecond;
}

void VectorEmitter::setParticleLifetime(thor::Distribution<sf::Time> particleLifetime)
{
    mParticleLifetime = std::move(particleLifetime);
}

void VectorEmitter::setParticlePosition(thor::Distribution<sf::Vector2f> particlePosition)
{
    mParticlePosition = std::move(particlePosition);
}

void VectorEmitter::setParticleVelocity(thor::Distribution<sf::Vector2f> particleVelocity)
{
    mParticleVelocity = std::move(particleVelocity);
}

void VectorEmitter::setParticleRotation(thor::Distribution<float> particleRotation)
{
    mParticleRotation = std::move(particleRotation);
}

void VectorEmitter::setParticleRotationSpeed(thor::Distribution<float> particleRotationSpeed)
{
    mParticleRotationSpeed = std::move(particleRotationSpeed);
}

void VectorEmitter::setParticleScale(thor::Distribution<sf::Vector2f> particleScale)
{
    mParticleScale = std::move(particleScale);
}

void VectorEmitter::setParticleColor(thor::Distribution<sf::Color> particleColor)
{
    mParticleColor = std::move(particleColor);
}

void VectorEmitter::setParticleTextureIndex(thor::Distribution<unsigned int> particleTextureIndex)
{
    mParticleTextureIndex = std::move(particleTextureIndex);
}

std::size_t VectorEmitter::computeParticleCount(sf::Time dt)
{
    // We want to fulfill the desired particle rate as exact as possible. Since the amount of emitted particles per frame is
    // integral, we have to emit sometimes more and sometimes less. mParticleDifference takes care of the deviation each frame.
    float particleAmount = mEmissionRate * dt.asSeconds() + mEmissionDifference;
    std::size_t nbParticles = static_cast<std::size_t>(particleAmount);

    // Compute difference for next frame, return current amount
    mEmissionDifference = particleAmount - nbParticles;
    return nbParticles;
}