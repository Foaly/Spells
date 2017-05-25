/// Spells - a game about magic spells
/// Copyright (C) 2015 - 2017  Foaly

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

// File created by Maximilian on 26.07.2015.


#ifndef SPELLS_UTIL_H
#define SPELLS_UTIL_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/Color.hpp>

#include <Thor/Math/Distributions.hpp>

#include <cassert>
#include <vector>
#include <string>
#include <iterator>

namespace util
{
    namespace Distributions
    {
        ///
        /// \brief A uniform random distribution in a certain range of a circle. Comparable to a disk.
        ///
        thor::Distribution<sf::Vector2f> disk(float minRadius, float maxRadius, sf::Vector2f center = sf::Vector2f(0.f, 0.f));
        
        ///
        /// \brief A distribution towards a direction with a deviation of maxAngle
        ///
        thor::Distribution<sf::Vector2f> deflect( thor::Distribution<sf::Vector2f> direction, float maxAngle);
        
        ///
        /// \brief A uniform distribution between two vectors
        ///
        thor::Distribution<sf::Vector2f> uniform (sf::Vector2f min, sf::Vector2f max);

        ///
        /// \brief A constant "distribution". Always returns the same value
        ///
        thor::Distribution<sf::Time> constant(sf::Time time);
    }
}


std::vector<sf::Vector2f> loadPathsFromFile(std::string filename);


///
/// \brief Returns a container with every n element of the input container
///
template<class in_it, class out_it>
out_it downsample( in_it begin, in_it end, out_it result, std::size_t n)
{
    for (std::size_t i = std::distance(begin, end) / n; i--; std::advance(begin, n))
        *result++ = *begin;
    return result;
}


template <typename T>
T clamp(T value, T min, T max) {
    assert(min < max);
    return std::max(min, std::min(value, max));
}


#endif //SPELLS_UTIL_H
