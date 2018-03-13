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

// File created by Maximilian on 11.03.2018.

#ifndef SPELLS_ELLIPSE_HPP
#define SPELLS_ELLIPSE_HPP

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Vertex.hpp>

#include <vector>

class Ellipse : public sf::Drawable, public sf::Transformable
{
public:
    Ellipse(float a1, float a2, float b, std::size_t pointCount);
    void setA1(float a1);
    void setA2(float a2);
    void setB(float b);


private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void updateVertices();

    float                      m_a1;         ///< First horizontal radius
    float                      m_a2;         ///< Second horizontal radius
    float                      m_b;          ///< Vertical radius
    std::size_t                m_pointCount; ///< Number of vertices on one half of the eclipse
    std::vector<sf::Vertex>    m_vertices;
};

#endif // SPELLS_ELLIPSE_HPP
