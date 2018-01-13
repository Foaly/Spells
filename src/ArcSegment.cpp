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

#include "ArcSegment.hpp"
#include "Util.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/System/Vector2.hpp>

#include <Thor/Math/Trigonometry.hpp>

#include <cmath>


ArcSegment::ArcSegment(float radius, float width) :
    m_radius(radius),
    m_width(width)
{
    // create a circle
    std::vector<sf::Vector2f> circle(30);
    for (int i = 0; i < circle.size(); ++i)
    {
        float angle = i * -2.f * thor::Pi / circle.size() - (thor::Pi / 2);
        float x = std::cos(angle) * radius;
        float y = std::sin(angle) * radius;
        circle[i] = sf::Vector2f(radius + x, radius + y);
    }
    circle.push_back(circle.front());

    // TODO: width calculation is still kinda fucky, but close enough
    float outterWidth = std::ceil(width / 4.f);
    float innerWidth = std::floor(width / 4.f);

    // create the donut shape
    m_vertices.resize(circle.size() * 2);
    for (int i = 0; i < circle.size(); ++i)
    {
        // Get the two segments shared by the current point
        sf::Vector2f p0 = (i == 0) ? circle.back() : circle[i - 1];
        sf::Vector2f p1 = circle[i];
        sf::Vector2f p2 = (i == circle.size() - 1) ? circle.front() : circle[i + 1];

        // Compute their normal
        sf::Vector2f n1 = computeNormal(p0, p1);
        sf::Vector2f n2 = computeNormal(p1, p2);

        // Combine them to get the extrusion direction
        float factor = 1.f + (n1.x * n2.x + n1.y * n2.y);
        sf::Vector2f normal = (n1 + n2) / factor;

        // generate the points
        m_vertices[i * 2 + 0].position = p1 - normal * outterWidth; // outside
        m_vertices[i * 2 + 1].position = p1 + normal * innerWidth; // inside
    }

    m_vertices.push_back(m_vertices[0].position);
    m_vertices.push_back(m_vertices[1].position);

    computeBounds();
}


const sf::FloatRect& ArcSegment::getLocalBounds() const
{
    return m_bounds;
}


void ArcSegment::setColor(sf::Color color)
{
    for(auto& vertex: m_vertices)
        vertex.color = color;
}


void ArcSegment::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();

    target.draw(m_vertices.data(), m_vertices.size(), sf::TrianglesStrip, states);
}


void ArcSegment::computeBounds()
{
    /*float left   = m_vertices[0].position.x;
    float top    = m_vertices[0].position.y;
    float right  = m_vertices[0].position.x;
    float bottom = m_vertices[0].position.y;

    for (std::size_t i = 1; i < m_vertices.size(); ++i)
    {
        sf::Vector2f position = m_vertices[i].position;

        // Update left and right
        if (position.x < left)
            left = position.x;
        else if (position.x > right)
            right = position.x;

        // Update top and bottom
        if (position.y < top)
            top = position.y;
        else if (position.y > bottom)
            bottom = position.y;
    }

    m_bounds = sf::FloatRect(left, top, right - left, bottom - top);*/
    float diameter = 2.f * m_radius + std::ceil(m_width / 4.f) * 2.f;
    m_bounds = sf::FloatRect(0.f, 0.f, diameter, diameter);

}
