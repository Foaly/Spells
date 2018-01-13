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


Clock::Clock(thor::ResourceHolder<sf::Texture, std::string>& textureHolder) :
    m_textureHolder(textureHolder),
    m_clockSprite(m_textureHolder["clock.png"]),
    m_arc(48.f, 9.f)
{
    sf::Vector2f halfSize = { m_arc.getLocalBounds().width / 2, m_arc.getLocalBounds().height / 2 };
    m_arc.setOrigin(halfSize);
    m_arc.setPosition(sf::Vector2f(112, 166));
    m_arc.setColor(sf::Color(30, 175, 11)); // green
}


const sf::Vector2f Clock::getSize() const
{
    return sf::Vector2f(m_clockSprite.getTexture()->getSize());
}


void Clock::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_clockSprite, states);
    target.draw(m_arc, states);
}
