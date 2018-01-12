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


#ifndef SPELLS_CLOCK_H
#define SPELLS_CLOCK_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include <Thor/Resources/ResourceHolder.hpp>


class Clock : public sf::Drawable, public sf::Transformable
{
public:
    Clock(thor::ResourceHolder<sf::Texture, std::string>& textureHolder);
    const sf::Vector2f getSize() const;

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

    thor::ResourceHolder<sf::Texture, std::string>&    m_textureHolder;
    sf::Sprite                                         m_clockSprite;
};



#endif //SPELLS_CLOCK_H
