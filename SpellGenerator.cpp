//
// Created by Maximilian on 21.06.2015.
//

#include "SpellGenerator.hpp"
#include <Thor/Math/Trigonometry.hpp>


SpellGenerator::SpellGenerator(sf::Vector2f center) :
        m_center(center)
{

}


std::vector<sf::Vector2f> SpellGenerator::generateSpirale()
{
    // algorithm taken from: https://stackoverflow.com/questions/13894715/draw-equidistant-points-on-a-spiral
    std::vector<sf::Vector2f> result;
    result.reserve(100);

    // number of coils or full rotations. (Positive numbers spin clockwise, negative numbers spin counter-clockwise)
    const float coils = 2.f;
    // distance from origin to outer arm
    const float radius = 300.f;
    // overall rotation of the spiral in radians
    const float rotation = 0.0;


    // value of theta corresponding to end of last coil
    const float thetaMax = coils * 2.f * thor::Pi;

    // How far to step away from center for each side.
    const float awayStep = radius / thetaMax;

    // distance between points to plot
    const float chord = 20.f;

    // put a point in center
    result.push_back(m_center);

    // For every side, step around and away from center.
    // start at the angle corresponding to a distance of chord
    // away from centre.
    for ( float theta = chord / awayStep; theta <= thetaMax; )
    {
        // How far away from center
        const float away = awayStep * theta;

        // How far around the center.
        const float around = theta + rotation;

        // Convert 'around' and 'away' to X and Y.
        const float x = m_center.x + std::cos ( around ) * away;
        const float y = m_center.y + std::sin ( around ) * away;

        // save the position
        result.push_back(sf::Vector2f(x, y));

        // to a first approximation, the points are on a circle
        // so the angle between them is chord/radius
        theta += chord / away;
    }

    return result;
}


std::vector<sf::Vector2f> SpellGenerator::generateWave()
{
    // algorithm taken from: https://stackoverflow.com/questions/26226663/evenly-space-circles-along-sin-curve#answer-26226795
    std::vector<sf::Vector2f> result;
    result.reserve(100);

    const float dx = 0.01f;  // increase for performance vs accuracy
    float s = 0.f;
    const float step = 0.2f; // equals a distance between points along the arc of 20 px
    float total = 0.f;

    for (float x = 0.f; x < 2.0 * thor::Pi; x += dx) {

        // method using difference between successive y values to calculate dy
        // NB: in practise you should remember the value of fx from the previous
        // iteration to avoid two evaluations of f(x) per loop
        const float fx = std::sin(x);
        const float dy = std::sin(x + dx) - fx;
        const float ds = (dx * dx + dy * dy) / dx;

        // add up distance so far
        s += ds;
        total += ds;

        if (s >= step) {
            const float px = 80.f + 800.f * (x / (2.f * thor::Pi));
            const float py = 380.f - 180.f * fx;

            result.push_back({px, py});
            s -= step;   // reset segment distance
        }
    }

    return result;
}

