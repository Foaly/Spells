//
// Created by Maximilian on 26.07.2015.
//

#ifndef SPELLS_UTIL_H
#define SPELLS_UTIL_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <Thor/Math/Distributions.hpp>
#include <Thor/Math/Random.hpp>
#include <Thor/Vectors/VectorAlgebra2D.hpp>
#include <Thor/Vectors/PolarVector2.hpp>

#include <cassert>

namespace util
{
    namespace Distributions
    {
        ///
        /// \brief A uniform random distribution in a certain range of a circle. Comparable to a disk.
        ///
        thor::Distribution<sf::Vector2f> disk(float minRadius, float maxRadius, sf::Vector2f center = sf::Vector2f(0.f, 0.f));
        
        ///
        /// \brief A constant "distribution". Always returns the same value
        ///
        thor::Distribution<sf::Time> constant(sf::Time time);
    }
}


#endif //SPELLS_UTIL_H
