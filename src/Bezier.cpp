#include "Bezier.hpp"

#include <cmath>
#include <cassert>


////////////////////////////////////////////////////////////////
/// @brief Bezier Curve
/// @param p0 Start point of the curve
/// @param p1 Tangent of the start point
/// @param p2 Tangent of the end point
/// @param p3 End point of the curve
////////////////////////////////////////////////////////////////
BezierCurve::BezierCurve(sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f p2, sf::Vector2f p3) :
    m_p0(p0),
    m_p1(p1),
    m_p2(p2),
    m_p3(p3)
{
    
}


////////////////////////////////////////////////////////////////
/// @brief Calculates a given number of point on the curve.
///        The interpolation parameter t is divided into equal steps
///        That results in the distance between points being dependant on the curvature of the curve
////////////////////////////////////////////////////////////////
std::vector<sf::Vector2f> BezierCurve::calculatePoints(std::size_t pointCount)
{
    assert(pointCount > 1);
    
    std::vector<sf::Vector2f> result;
    result.reserve(pointCount);
    
    for(std::size_t i = 0; i < pointCount; i++)
    {
        float t = static_cast<float>(i) / (pointCount - 1);
        
        // Bernstein polynomial
        sf::Vector2f point = std::pow(1.f - t, 3.f) * m_p0 +
                             3.f * std::pow(1.f - t, 2.f) * t * m_p1 +
                             3.f * (1 - t) * std::pow(t, 2.f) * m_p2 +
                             std::pow(t, 3.f) * m_p3;
        result.push_back(point);
    }
    
    return result;
}
