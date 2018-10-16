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
#include "Util.hpp"

#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <Thor/Math/Trigonometry.hpp>
#include <Thor/Math/Random.hpp>

#include <cmath>
#include <chrono>
#include <ctime>


namespace {
    /*
     * Returns the phase of the moon. Algorithm from John Conway.
     * Approximation apparently only valid for the 20th and 21st centuries...
     * Taken from http://www.ben-daglish.net/moon.shtml
     * Returns 0-29, where 0 = new moon, 1-14 waxing, 15 = full, 15-29 waning
     */
    int moonphaseConway(int year, int month, int day)
    {
        int r = year % 100;
        r %= 19;
        if (r>9){ r -= 19;}
        r = ((r * 11) % 30) + month + day;
        if (month<3){r += 2;}
        r -= ((year<2000) ? 4 : 8.3);
        r = static_cast<int>(std::floor(r+0.5)) % 30;
        return (r < 0) ? r+30 : r;
    }

    const float twoPi = 2.f * thor::Pi;
}


Clock::Clock(thor::ResourceHolder<sf::Texture, std::string>& textureHolder) :
    m_clockSprite(textureHolder["clock.png"]),
    m_arc(48.f, 9.f),
    m_origin(109, 162),
    m_planetRadius(34.f),
    m_jupiterSprite(textureHolder["jupiter.png"]),
    m_jupiterAngle(thor::random(-twoPi, 0.f)),
    m_uranusSprite(textureHolder["uranus.png"]),
    m_uranusAngle(thor::random(0.f, twoPi)),
    m_moonShadow(-21, 23, 22, 30)
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
    m_uranusSprite.setOrigin(sf::Vector2f(textureHolder["uranus.png"].getSize()) / 2.f);

    m_moonShadow.setPosition(m_origin + sf::Vector2f(-1.f, 1.f));
    m_moonShadow.rotate(-30);

    // get the local date
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = std::localtime(&in_time_t);

    // get the real current moon phase and set the moon shadow accordingly
    float phase = moonphaseConway( 1900 + tm->tm_year, tm->tm_mon + 1, tm->tm_mday) / 29.f;
    phase = clamp(phase, 0.f, 1.f);
    if (phase <= 0.5f)
    {
        // waxing moon
        m_moonShadow.setA1(-21);
        m_moonShadow.setA2( util::linearInterpolation(23, -23, phase * 2.f) );
    }
    else
    {
        // waning moon
        m_moonShadow.setA1( util::linearInterpolation(21, -21, (phase - 0.5f) * 2.f) );
        m_moonShadow.setA2(23);
    }
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
    if (m_jupiterAngle < -twoPi)
        m_jupiterAngle = 0.f;

    auto jupiterPosition = m_origin + m_planetRadius * sf::Vector2f(std::cos(m_jupiterAngle), std::sin(m_jupiterAngle));
    m_jupiterSprite.setPosition(jupiterPosition);

    m_uranusAngle += 0.1f * frameTime.asSeconds();
    if (m_uranusAngle > twoPi)
        m_uranusAngle = 0.f;

    auto uranusPostion = m_origin + m_planetRadius * sf::Vector2f(std::cos(m_uranusAngle), std::sin(m_uranusAngle));
    m_uranusSprite.setPosition(uranusPostion);
}


void Clock::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_clockSprite, states);
    target.draw(m_moonShadow, states);
    target.draw(m_arc, states);
    target.draw(m_jupiterSprite, states);
    target.draw(m_uranusSprite, states);
}
