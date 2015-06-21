//
// Created by Maximilian on 21.06.2015.
//
#define _USE_MATH_DEFINES
#include <cmath>

#include "SpellGenerator.h"

SpellGenerator::SpellGenerator(sf::Vector2f center) : m_center(center)
{

}


std::vector<sf::CircleShape> SpellGenerator::generateSpirale()
{
    // algorithm taken from: https://stackoverflow.com/questions/13894715/draw-equidistant-points-on-a-spiral
    std::vector<sf::CircleShape> result;
    result.reserve(100);

    // number of coils or full rotations. (Positive numbers spin clockwise, negative numbers spin counter-clockwise)
    const float coils = 2.f;
    // distance from origin to outer arm
    const float radius = 300.f;
    // overall rotation of the spiral in radians
    const double rotation = 0.0;


    // value of theta corresponding to end of last coil
    const double thetaMax = coils * 2 * M_PI;

    // How far to step away from center for each side.
    const double awayStep = radius / thetaMax;

    // distance between points to plot
    const double chord = 20;

    // put a point in center
    result.push_back(createCircle(m_center));

    // For every side, step around and away from center.
    // start at the angle corresponding to a distance of chord
    // away from centre.
    for ( double theta = chord / awayStep; theta <= thetaMax; )
    {
        // How far away from center
        double away = awayStep * theta;

        // How far around the center.
        double around = theta + rotation;

        // Convert 'around' and 'away' to X and Y.
        double x = m_center.x + std::cos ( around ) * away;
        double y = m_center.y + std::sin ( around ) * away;

        // save the position
        result.push_back(createCircle(sf::Vector2f(x, y)));

        // to a first approximation, the points are on a circle
        // so the angle between them is chord/radius
        theta += chord / away;
    }

    return result;
}

sf::CircleShape SpellGenerator::createCircle(sf::Vector2f position) const
{
    sf::CircleShape circle;
    circle.setRadius(25);
    circle.setPosition(position);
    circle.setFillColor(sf::Color(255, 255, 255, 100));
    return circle;
}
