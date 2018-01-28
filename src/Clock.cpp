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

// File created by Maximilian on 12.01.2018.

#include "Clock.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <Thor/Math/Trigonometry.hpp>
#include <Thor/Math/Random.hpp>

#include <cmath>


Clock::Clock(thor::ResourceHolder<sf::Texture, std::string>& textureHolder) :
    m_clockSprite(textureHolder["clock.png"]),
    m_jupiterSprite(textureHolder["jupiter.png"]),
    m_arc(48.f, 9.f),
    m_origin(108, 162),
    m_planetRadius(34.f),
    m_jupiterAngle(thor::random(-2.f * thor::Pi, 0.f))
{
    m_gradient[0.0f] = sf::Color( 20, 173,  0); // green
    m_gradient[0.4f] = sf::Color(220, 232, 13); // yellow
    m_gradient[0.5f] = sf::Color(239, 167,  0); // orange
    m_gradient[0.9f] = sf::Color(198,   0,  0); // red
    m_gradient[1.0f] = sf::Color(198,   0,  0); // red

    sf::Vector2f halfSize = { m_arc.getCircleSize().width / 2, m_arc.getCircleSize().height / 2 };
    m_arc.setOrigin(halfSize);
    m_arc.setPosition(sf::Vector2f(112, 165));
    m_arc.setColor(m_gradient.sampleColor(0.f));

    m_jupiterSprite.setOrigin(sf::Vector2f(textureHolder["jupiter.png"].getSize()) / 2.f);
    m_jupiterSprite.setPosition(108, 128);
}


const sf::Vector2f Clock::getSize() const
{
    return sf::Vector2f(m_clockSprite.getTexture()->getSize());
}


void Clock::update(sf::Time frameTime)
{
    if (m_clock.getElapsedTime() < sf::seconds(8.f))
    {
        float amount = m_clock.getElapsedTime() / sf::seconds(8.f);
        m_arc.setArcLength(amount);
        m_arc.setColor(m_gradient.sampleColor(amount));
    }
    else
    {
        m_clock.restart();
    }

    m_jupiterAngle -= 0.3f * frameTime.asSeconds();
    if (m_jupiterAngle < -2.f * thor::Pi)
        m_jupiterAngle = 0.f;

    auto jupiterPosition = m_origin + m_planetRadius * sf::Vector2f(std::cos(m_jupiterAngle), std::sin(m_jupiterAngle));
    m_jupiterSprite.setPosition(jupiterPosition);
}


void Clock::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_clockSprite, states);
    target.draw(m_arc, states);
    target.draw(m_jupiterSprite, states);
}
