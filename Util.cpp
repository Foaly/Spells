//
// Created by Maximilian on 26.07.2015.
//

#include "Util.hpp"

#include <iostream>

thor::Distribution<sf::Vector2f> util::Distributions::disk(sf::Vector2f center, float minRadius, float maxRadius)
{
    assert(minRadius >= 0.f);
    assert(maxRadius > minRadius);

    return [=] () -> sf::Vector2f
    {
        sf::Vector2f radiusVector = thor::PolarVector2f(thor::random(minRadius, maxRadius), thor::random(0.f, 360.f));
        return center + radiusVector;
    };
}