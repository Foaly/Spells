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
std::vector<sf::Vector2f> BezierCurve::generateRegularPoints(std::size_t pointCount)
{
    assert(pointCount > 1);
    
    std::vector<sf::Vector2f> result;
    result.reserve(pointCount);
    
    for(std::size_t i = 0; i < pointCount; i++)
    {
        float t = static_cast<float>(i) / (pointCount - 1);
        result.push_back(calculatePoint(t));
    }
    
    return result;
}


////////////////////////////////////////////////////////////////
/// @brief Calculates the corresponding point on the curve for the interpolation
///        parameter t. t must be between 0 and 1. 0 corresponds to m_p0 and 1 to m_p3.
////////////////////////////////////////////////////////////////
sf::Vector2f BezierCurve::calculatePoint(float t)
{
    // Bernstein polynomial
    return std::pow(1.f - t, 3.f) * m_p0 +
           3.f * std::pow(1.f - t, 2.f) * t * m_p1 +
           3.f * (1 - t) * std::pow(t, 2.f) * m_p2 +
           std::pow(t, 3.f) * m_p3;
}


////////////////////////////////////////////////////////////////
/// @brief Generates points with a constant distance between each other along the curve
////////////////////////////////////////////////////////////////
std::vector<sf::Vector2f> BezierCurve::generateEvenlySpacedPoints(float distanceBetweenPoints)
{
    assert(distanceBetweenPoints >= 1.f);
    
    // The following solution to the problem of putting points with even distributed
    // distances along a curve was solved with this very helpful post:
    // https://gamedev.stackexchange.com/questions/5373/moving-ships-between-two-planets-along-a-bezier-missing-some-equations-for-acce/5427#5427
    
    const float numberOfSegments = 100.f; // this should never be below 1
    std::vector<float> segmentLengths(numberOfSegments + 1);
    segmentLengths[0] = 0.f;
    
    // precalculate a number of points on the curve
    // measure the distance between them and store it
    float currentLength = 0.f;
    sf::Vector2f previousPoint = calculatePoint(0.f);
    for(std::size_t i = 1; i <= numberOfSegments; i++)
    {
        sf::Vector2f currentPoint = calculatePoint(i / (numberOfSegments - 1));
        sf::Vector2f delta = previousPoint - currentPoint;
        currentLength += std::sqrt(delta.x * delta.x + delta.y * delta.y);
        segmentLengths[i] = currentLength;
        previousPoint = currentPoint;
    }
    const float curveLength = currentLength;
    
    // map the normalized length on the curve back onto the interpolation parameter t
    auto map = [&](float u)
    {
        float targetLength = u * segmentLengths[numberOfSegments];
        unsigned int low = 0;
        unsigned int high = numberOfSegments;
        unsigned int index = 0;
        while (low < high)
        {
            index = low + static_cast<unsigned int>((high - low) / 2);
            if (segmentLengths[index] < targetLength) {
                low = index + 1;
                
            } else {
                high = index;
            }
        }
        if (segmentLengths[index] > targetLength) {
            index--;
        }
        
        float lengthBefore = segmentLengths[index];
        if (lengthBefore == targetLength) {
            return index / numberOfSegments;
        } else {
            return (index + (targetLength - lengthBefore) / (segmentLengths[index + 1] - lengthBefore)) / numberOfSegments;
        }
    };
    
    assert(distanceBetweenPoints < curveLength);

    // compute how many segments of a given length fit on the curve
    std::size_t pointCount = std::floor(curveLength / distanceBetweenPoints);
    
    // compute the points on the curve
    std::vector<sf::Vector2f> result;
    result.reserve(pointCount);
    for(std::size_t i = 0; i < pointCount; i++)
    {
        float u = static_cast<float>(i) / (pointCount - 1);
        result.push_back(calculatePoint(map(u)));
    }

    return result;
}
