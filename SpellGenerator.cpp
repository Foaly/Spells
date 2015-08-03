//
// Created by Maximilian on 21.06.2015.
//
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "SpellGenerator.hpp"
#include <Thor/Resources/SfmlLoaders.hpp>


SpellGenerator::SpellGenerator(sf::Vector2f center, thor::ResourceHolder<sf::Texture, std::string>& texture) :
        m_center(center),
        m_textures(texture)
{
    try
    {
        m_textures.acquire("circle", thor::Resources::fromFile<sf::Texture>("data/textures/Circle.png"), thor::Resources::Reuse);
    }
    catch (thor::ResourceLoadingException &e)
    {
        std::cout << e.what() << std::endl;
    }
}


std::vector<sf::Sprite> SpellGenerator::generateSpirale()
{
    // algorithm taken from: https://stackoverflow.com/questions/13894715/draw-equidistant-points-on-a-spiral
    std::vector<sf::Sprite> result;
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
        float x = m_center.x + std::cos ( around ) * away;
        float y = m_center.y + std::sin ( around ) * away;

        // save the position
        result.push_back(createCircle(sf::Vector2f(x, y)));

        // to a first approximation, the points are on a circle
        // so the angle between them is chord/radius
        theta += chord / away;
    }

    return result;
}


std::vector<sf::Sprite> SpellGenerator::generateWave()
{
    // algorithm taken from: https://stackoverflow.com/questions/26226663/evenly-space-circles-along-sin-curve#answer-26226795
    std::vector<sf::Sprite> result;
    result.reserve(100);

    const float dx = 0.01f;  // increase for performance vs accuracy
    float s = 0.f;
    const float step = 0.2f; // equals a distance between points along the arc of 20 px
    float total = 0.f;

    for (float x = 0.f; x < 2.0 * M_PI; x += dx) {

        // method using difference between successive y values to calculate dy
        // NB: in practise you should remember the value of fx from the previous
        // iteration to avoid two evaluations of f(x) per loop
        float fx = std::sin(x);
        float dy = std::sin(x + dx) - fx;
        float ds = (dx * dx + dy * dy) / dx;

        // add up distance so far
        s += ds;
        total += ds;

        if (s >= step) {
            float px = 80.f + 800.f * (x / (2.f * static_cast<float>(M_PI)));
            float py = 380.f - 180.f * fx;

            result.push_back(createCircle({px, py}));
            s -= step;   // reset segment distance
        }
    }

    return result;
}

sf::Sprite SpellGenerator::createCircle(sf::Vector2f position) const
{
    sf::Sprite circle(m_textures["circle"]);
    circle.setOrigin(25, 25);
    circle.setPosition(position);
    circle.setColor(sf::Color(255, 255, 255, 100));
    return circle;
}
