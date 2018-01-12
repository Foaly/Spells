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

#ifndef SPELLS_BEZIER_H
#define SPELLS_BEZIER_H

#include <SFML/System/Vector2.hpp>

#include <vector>

class BezierCurve
{
public:
    BezierCurve(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3);

    std::vector<sf::Vector2f> generateRegularPoints(std::size_t pointCount);
    sf::Vector2f              calculatePoint(float t);
    std::vector<sf::Vector2f> generateEvenlySpacedPoints(float distanceBetweenPoints);

private:

    sf::Vector2f m_p0;
    sf::Vector2f m_p1;
    sf::Vector2f m_p2;
    sf::Vector2f m_p3;
};


#endif // SPELLS_BEZIER_H
