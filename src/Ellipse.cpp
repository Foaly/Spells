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

#include "Ellipse.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include <Thor/Math/Trigonometry.hpp>

#include <cassert>
#include <cmath>


namespace {
    const float pi     = thor::Pi;
    const float halfPi = pi / 2.f;
}


Ellipse::Ellipse(float a1, float a2, float b, std::size_t pointCount):
    m_a1(a1),
    m_a2(a2),
    m_b(b),
    m_pointCount(pointCount)
{
    assert(m_pointCount > 2);
    updateVertices();
}


void Ellipse::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(m_vertices.data(), m_vertices.size(), sf::TriangleStrip, states);
}


void Ellipse::updateVertices()
{
    m_vertices.clear();
    m_vertices.reserve(m_pointCount);

    // begin
    float x = m_a1 * std::cos( -halfPi );
    float y = m_b * std::sin( -halfPi );
    m_vertices.push_back(sf::Vertex(sf::Vector2f(x, y), sf::Color::Black));

    const float step = pi / (m_pointCount - 1);
    for (std::size_t i = 1; i < m_pointCount - 1; ++i)
    {
        float t = i * step - halfPi;
        x = m_a1 * std::cos(t);
        y = m_b * std::sin(t);
        m_vertices.push_back(sf::Vertex(sf::Vector2f(x, y), sf::Color::Black));

        x = m_a2 * std::cos(t);
        y = m_b * std::sin(t);
        m_vertices.push_back(sf::Vertex(sf::Vector2f(x, y), sf::Color::Black));
    }

    // end
    x = m_a1 * std::cos(pi - halfPi);
    y = m_b * std::sin(pi - halfPi);
    m_vertices.push_back(sf::Vertex(sf::Vector2f(x, y), sf::Color::Black));
}

void Ellipse::setA1(float a1)
{
    m_a1 = a1;
    updateVertices();
}

void Ellipse::setA2(float a2)
{
    m_a2 = a2;
    updateVertices();
}

void Ellipse::setB(float b)
{
    m_b = b;
    updateVertices();
}
