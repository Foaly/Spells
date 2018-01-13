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


#ifndef SPELLS_ARCSEGMENT_H
#define SPELLS_ARCSEGMENT_H

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <Thor/Resources/ResourceHolder.hpp>

#include <vector>


class ArcSegment : public sf::Drawable, public sf::Transformable
{
public:
    ArcSegment(float radius, float width);
    const sf::FloatRect& getLocalBounds() const;
    void setColor(sf::Color color);

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    void computeBounds();

    std::vector<sf::Vertex>    m_vertices;
    sf::FloatRect              m_bounds;
    float                      m_radius;
    float                      m_width;
};



#endif //SPELLS_ARCSEGMENT_H
